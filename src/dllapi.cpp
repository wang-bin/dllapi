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
#include <QtCore/QLibrary>

/*TODO:
 *
 * 1. search path implemention
 * 2. lib depended search dirs
 */

namespace DllAPI {
static std::list<std::string> sLibDirs;

void setSearchPaths(const std::list<std::string>& paths)
{
    sLibDirs = paths;
}

void addSearchPaths(const std::list<std::string>& paths)
{
    for (std::list<std::string>::const_iterator it = paths.cbegin();
         it != paths.cend(); ++it) {
        if (std::find(sLibDirs.cbegin(), sLibDirs.cend(), *it) == sLibDirs.cend()) {
            sLibDirs.push_back(*it);
        }
    }
}

void removeSearchPaths(const std::list<std::string> &paths)
{
    for (std::list<std::string>::const_iterator it = paths.cbegin();
         it != paths.cend(); ++it) {
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
    for (std::list<std::string>::const_iterator it = names.cbegin();
         it != names.cend(); ++it) {
        if (std::find(libnames.cbegin(), libnames.cend(), *it) == libnames.cend()) {
            libnames.push_back(*it);
        }
    }
}

void removeLibraryNames(const std::string& lib, const std::list<std::string>& names)
{
    std::list<std::string> &libnames = sLibNamesMap[lib];
    for (std::list<std::string>::const_iterator it = names.cbegin();
         it != names.cend(); ++it) {
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


typedef std::map<std::string, QLibrary*> dllmap_t;
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
    QLibrary *dll = new QLibrary();
    for (it = libnames.cbegin(); it != libnames.cend(); ++it) {
        dll->setFileName((*it).c_str());
        if (dll->load())
            break;
        DBG("%s\n", dll->errorString().toLocal8Bit().constData()); //why qDebug use toUtf8() and printf use toLocal8Bit()?
    }
    if (it == libnames.cend()) {
        DBG("No dll loaded\n");
        delete dll;
        return false;
    }
    DBG("'%s' is loaded~~~\n", dll->fileName().toUtf8().constData());
    sDllMap[dllname] = dll; //map.insert will not replace the old one
    return true;
#if 0
    QLibrary *dll = new QLibrary(dllname);
    if (!dll)
        return false;
    if (!dll->load()) {
        DBG("%s\n", dll->errorString().toLocal8Bit().constData()); //why qDebug use toUtf8() and printf use toLocal8Bit()?
        return false;
    }
    sDllMap[dllname] = dll; //map.insert will not replace the old one
    return true;
#endif
}

template<class StdMap>
class map_value_equal : public std::binary_function<typename StdMap::value_type, typename StdMap::mapped_type, bool>
{ //why is value_type but not const_iterator?
public:
    bool operator() (typename StdMap::value_type it, typename StdMap::mapped_type v) const {return it.second == v;}
};

bool unload(const char* dllname)
{
    QLibrary *dll = library(dllname);
    if (!dll) {
        DBG("'%s' is not loaded\n", dllname);
        return true;
    }
    if (!dll->unload()) {
        DBG("%s", dll->errorString().toUtf8().constData());
        return false;
    }
    sDllMap.erase(std::find_if(sDllMap.begin(), sDllMap.end(), std::bind2nd(map_value_equal<dllmap_t>(), dll))->first);
    delete dll;
    return true;
}

QLibrary* library(const char *dllname)
{
    dllmap_t::iterator it = sDllMap.find(dllname);
    if (it == sDllMap.end())
        return 0;
    return it->second;
}

} //namespace DllAPI
