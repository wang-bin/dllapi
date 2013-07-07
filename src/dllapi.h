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

#ifndef DLLAPI_H
#define DLLAPI_H

/*!
 * put "someprefix/somelib.h" and all required content in namespace DllAPI, in file "dllapi/someprefix/somelib.h"
 * e.g.
   namespace DllAPI {
   namespace SomeLib {
   extern CL_API_ENTRY cl_int CL_API_CALL
   clGetPlatformIDs(cl_uint  num_entries,
                    cl_platform_id *platforms,
                    cl_uint *num_platforms) CL_API_SUFFIX__VERSION_1_0;
   }
   }

   in "dllapi/someprefix/somelib.cpp":
   #include "dllapi.h"
   #include "dllapi_p.h"
   namespace DllAPI {
   namespace SomeLib {
    DEFINE_DLL_INSTANCE(libname)
    e.g. DEFINE_DLLAPI_ARG(3, cl_int, clGetPlatformIDs, cl_uint, cl_platform_id, cl_uint);
   }
   }

   then in your app source, just add:
   #include "dllapi/someprefix/somelib.h"
   using namespace DllAPI::SomeLib;
   to enable dynamically loading api instead of linked api. of cause, you should not link to that lib from then on.
*/
#include "dllapi_global.h"
#include <list>
#include <string>

namespace DllAPI {

Q_EXPORT void setSearchPaths(const std::list<std::string>& paths);
Q_EXPORT std::list<std::string> getSearchPaths();
/* use this to test whether the library can be loaded. It will try load if not loaded.
 *return true if already loaded or load success.
 * dllname can be standard name which is used as id internally, for example 'OpenAL' can be 'OpenAL' and 'OpenAL32'
 * , defined in OpenAL.cpp. Search in this id first. If found, use it. Otherwise go back to normal dll search by dllname
 *
 */
Q_EXPORT bool testLoad(const char* dllname);
/*
 * insert (std::string dllname, dllptr) to a map
 * they are used in xxx.cpp xxx.ctor/dtor for the most time
 * usually you don't need to call them
 */
Q_EXPORT bool load(const char* dllname);
Q_EXPORT bool unload(const char* dllname);

} //namespace DllAPI

#endif //DLLAPI_H

