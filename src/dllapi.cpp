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
#include <map>
#include <string>
#include <QtCore/QLibrary>

using namespace std;

namespace DllAPI {

typedef map<std::string, QLibrary*> dllmap_t;
static dllmap_t sDllMap;

bool load(const char* dllname)
{
    QLibrary *dll = new QLibrary(dllname) ;
    if (!dll)
        return false;
    if (!dll->load()) {
        qDebug("%s", dll->errorString().toUtf8().constData());
        return false;
    }
    sDllMap[dllname] = dll; //map.insert will not replace the old one
    return true;
}

bool unload(const char* dllname)
{
    dllmap_t::iterator it = sDllMap.find(dllname);
    if (it == sDllMap.end())
        return false;
    if (!it->second->unload()) {
        qDebug("%s", it->second->errorString().toUtf8().constData());
        return false;
    }
    sDllMap.erase(it);
    return true;
}

QLibrary* library(const char *dllname)
{
    dllmap_t::iterator it = sDllMap.find(dllname);
    if (it == sDllMap.end())
        return 0;
    return it->second;
}

}
