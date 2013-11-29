/******************************************************************************
    DllAPI: call c api by dynamically loading it's library
    Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    Alternatively, this file may be used under the terms of the GNU
    General Public License version 3.0 as published by the Free Software
    Foundation and appearing in the file LICENSE.GPL included in the
    packaging of this file.  Please review the following information to
    ensure the GNU General Public License version 3.0 requirements will be
    met: http://www.gnu.org/copyleft/gpl.html.
******************************************************************************/

#include "dllapi.h"
#include "dllapi_p.h"
#include <algorithm>
#include <functional>
#include <map>
#include <string>

#if defined(Q_OS_WIN)
const std::string kDllPrefix = "";
const std::string kDllSuffix = ".dll";
#elif defined(Q_OS_MAC)
#include <dlfcn.h>
const std::string kDllPrefix = "lib";
const std::string kDllSuffix = ".dylib";
#else
#include <dlfcn.h>
const std::string kDllPrefix = "lib";
const std::string kDllSuffix = ".so";
#endif

/*TODO:
 *
 * 1. search path implemention. or use qApp->addLibraryPath() out side
 * 2. lib depended search dirs
 */

namespace DllAPI {

void DllObject::setFileName(const std::string &name)
{
    file = name;
    if (name.size() < kDllSuffix.size()) {
        file += kDllSuffix;
        return;
    }
    if (name.substr(name.size() - kDllSuffix.size()) != kDllSuffix) {
        file += kDllSuffix;
        return;
    }
}

bool DllObject::load()
{
#ifdef Q_OS_WIN
    return _load(false);
#else
    return _load(true);
#endif
}

bool DllObject::_load(bool tryprefix)
{
#if defined(Q_OS_WIN)

#else
    handle = dlopen(file.c_str(), RTLD_NOW|RTLD_LOCAL);
    if (!handle) {
        error = dlerror();
        if (file.substr(0, kDllPrefix.size()) != kDllPrefix) {
            file = kDllPrefix + file;
            return _load(false);
        }
    } else {
        error.clear();
    }
#endif
    return !!handle;
}

bool DllObject::unload()
{
#if defined(Q_OS_WIN)

#else
    if (handle) {
        int err = dlclose(handle);
        if (err != 0) {
            error = dlerror();
        } else {
            error.clear();
            handle = 0;
        }
    }
#endif
    return !handle;
}

void* DllObject::resolve(const std::string &symb)
{
    return _reslove(symb, true);
}

void* DllObject::_reslove(const std::string &symb, bool again)
{
    void *symbol = 0;
#if defined(Q_OS_WIN)

#else
    symbol = dlsym(handle, symb.c_str());
    if (!symbol && again) {
        return _reslove("_" + symb, false);
    }
    if (!symbol) {
        error = dlerror();
    } else {
        error.clear();
    }
#endif
    return symbol;
}


static std::list<std::string> sLibDirs;

void setSearchPaths(const std::list<std::string>& paths)
{
    sLibDirs = paths;
}

void addSearchPaths(const std::list<std::string>& paths)
{
    for (std::list<std::string>::const_iterator it = paths.begin();
         it != paths.end(); ++it) {
        if (std::find(sLibDirs.begin(), sLibDirs.end(), *it) == sLibDirs.end()) {
            sLibDirs.push_back(*it);
        }
    }
}

void removeSearchPaths(const std::list<std::string> &paths)
{
    for (std::list<std::string>::const_iterator it = paths.begin();
         it != paths.end(); ++it) {
        std::remove_if(sLibDirs.begin(), sLibDirs.end(), std::bind2nd(std::equal_to<std::string>(), *it));
    }
}

std::list<std::string> getSearchPaths()
{
    return sLibDirs;
}

typedef std::map<std::string, std::list<std::string> > lib_names_map_t;
static lib_names_map_t sLibNamesMap;
void setLibraryNames(const std::string& lib, const std::list<std::string>& names)
{
    sLibNamesMap[lib] = names;
}

void addLibraryNames(const std::string& lib, const std::list<std::string>& names)
{
    std::list<std::string> &libnames = sLibNamesMap[lib];
    if (libnames.empty())
        libnames.push_back(lib);
    for (std::list<std::string>::const_iterator it = names.begin();
         it != names.end(); ++it) {
        if (std::find(libnames.begin(), libnames.end(), *it) == libnames.end()) {
            libnames.push_back(*it);
        }
    }
}

void removeLibraryNames(const std::string& lib, const std::list<std::string>& names)
{
    std::list<std::string> &libnames = sLibNamesMap[lib];
    for (std::list<std::string>::const_iterator it = names.begin();
         it != names.end(); ++it) {
        std::remove_if(libnames.begin(), libnames.end(), std::bind2nd(std::equal_to<std::string>(), *it));
    }
}

std::list<std::string> getLibraryNames(const std::string& lib)
{
    return sLibNamesMap[lib];
}

bool testLoad(const char *dllname)
{
    if (library(dllname))
        return true;
    return load(dllname);
}


typedef std::map<std::string, DllObject*> dllmap_t;
static dllmap_t sDllMap;

bool load(const char* dllname)
{
    if (library(dllname)) {
        DBG("'%s' is already loaded\n", dllname);
        return true;
    }
    std::list<std::string> libnames = sLibNamesMap[dllname];
    if (libnames.empty())
        libnames.push_back(dllname);
    std::list<std::string>::const_iterator it;
    DllObject *dll = new DllObject();
    for (it = libnames.begin(); it != libnames.end(); ++it) {
        dll->setFileName((*it).c_str());
        // TODO: search paths
        if (dll->load())
            break;
        DBG("%s\n", dll->errorString().c_str()); //why qDebug use toUtf8() and printf use toLocal8Bit()?
    }
    if (it == libnames.end()) {
        DBG("No dll loaded\n");
        delete dll;
        return false;
    }
    DBG("'%s' is loaded~~~\n", dll->fileName().c_str());
    sDllMap[dllname] = dll; //map.insert will not replace the old one
    return true;
}

template<class StdMap>
class map_value_equal : public std::binary_function<typename StdMap::value_type, typename StdMap::mapped_type, bool>
{ //why is value_type but not const_iterator?
public:
    bool operator() (typename StdMap::value_type it, typename StdMap::mapped_type v) const {return it.second == v;}
};

bool unload(const char* dllname)
{
    DllObject *dll = library(dllname);
    if (!dll) {
        DBG("'%s' is not loaded\n", dllname);
        return true;
    }
    if (!dll->unload()) {
        DBG("%s", dll->errorString().c_str());
        return false;
    }
    sDllMap.erase(std::find_if(sDllMap.begin(), sDllMap.end(), std::bind2nd(map_value_equal<dllmap_t>(), dll))->first);
    delete dll;
    return true;
}

DllObject* library(const char *dllname)
{
    dllmap_t::iterator it = sDllMap.find(dllname);
    if (it == sDllMap.end())
        return 0;
    return it->second;
}

} //namespace DllAPI
