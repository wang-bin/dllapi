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
 * in dllapi/CL/cl.cpp, define the apis use macro
 *
 * in your application, use the api as usual, except that you should include headers like below:
 * #if CONFIG_DLLAPI_CL
 * #include <dllapi/CL/cl.h>
 * using namespace DllAPI:OpenCL;
 * #else
 * #include <CL/cl.h>
 * #endif
 *
 * TODO: study COM api
 */

#include "dllapi_global.h"
#include <stdio.h>


#if defined(_MSC_VER)
#pragma warning(disable:4098) //vc return void
#endif //_MSC_VER

class QLibrary;
namespace DllAPI {

/*
 *insert (std::string dllname, dllptr) to a map
 * they are used in xxx.cpp xxx.ctor/dtor for the most time
 *TODO: move load, unload, sDllMap, to dllapi_p.h(also map), keep this file empty?
 *if this is empty, then we don't need this. so we can move them here, and
 * this file(dllapi.h) is only used in xxx.cpp, e.g. dllapi/CL/cl.cpp
 * and dllapi/CL/cl.h is clean, only CL/cl.h content and DllAPI::OpenCL namespace
 */
bool load(const char* dllname);
bool unload(const char* dllname);
QLibrary* library(const char* dllname);

/*
 * put .h content here
 * e.g.
    extern CL_API_ENTRY cl_int CL_API_CALL
    clGetPlatformIDs(cl_uint  num_entries,
                    cl_platform_id *platforms,
                    cl_uint *num_platforms) CL_API_SUFFIX__VERSION_1_0;
*/

//e.g. DEFINE_DLLAPI_SYM_ARG(3, cl_int, clGetPlatformIDs, cl_uint, cl_platform_id*, cl_uint*)
#define DEFINE_DLLAPI_ARG(N, R, name, P) DEFINE_DLLAPI_SYM_ARG##N(R, name, #name, P)
/*
 * used by .cpp to define the api
 *  e.g. DEFINE_DLLAPI_SYM_ARG3(cl_int, clGetPlatformIDs, "clGetPlatformIDs", cl_uint, cl_platform_id*, cl_uint*)
 */
#define DEFINE_DLLAPI_SYM_ARG0(R, name, sym, P) DEFINE_DLLAPI_ARG(R, name, sym, (), (), ())
#define DEFINE_DLLAPI_SYM_ARG1(R, name, sym, P1) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1), (P1 p1), (p1))
#define DEFINE_DLLAPI_SYM_ARG2(R, name, sym, P1, P2) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2), (P1 p1, P2 p2), (p1, p2))
#define DEFINE_DLLAPI_SYM_ARG3(R, name, sym, P1, P2, P3) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3), (P1 p1, P2 p2, P3 p3), (p1, p2, p3))
#define DEFINE_DLLAPI_SYM_ARG4(R, name, sym, P1, P2, P3, P4) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4), (P1 p1, P2 p2, P3 p3, P4 p4), (p1, p2, p3, p4))
#define DEFINE_DLLAPI_SYM_ARG5(R, name, sym, P1, P2, P3, P4, P5) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4, P5), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5), (p1, p2, p3, p4, p5))
#define DEFINE_DLLAPI_SYM_ARG6(R, name, sym, P1, P2, P3, P4, P5, P6) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4, P5, P6), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6), (p1, p2, p3, p4, p5, p6))
#define DEFINE_DLLAPI_SYM_ARG7(R, name, sym, P1, P2, P3, P4, P5, P6, P7) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4, P5, P6, P7), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7), (p1, p2, p3, p4, p5, p6, p7))
#define DEFINE_DLLAPI_SYM_ARG8(R, name, sym, P1, P2, P3, P4, P5, P6, P7, P8) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8), (p1, p2, p3, p4, p5, p6, p7, p8))
#define DEFINE_DLLAPI_SYM_ARG9(R, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9), (p1, p2, p3, p4, p5, p6, p7, p8, p9))
#define DEFINE_DLLAPI_SYM_ARG10(R, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10))
#define DEFINE_DLLAPI_SYM_ARG11(R, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11))
#define DEFINE_DLLAPI_SYM_ARG12(R, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12))
#define DEFINE_DLLAPI_SYM_ARG13(R, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12, P13 p13), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13))

template<typename T>
struct TDefault {
    static T value = T();
};
template<typename T>
struct TDefault<T*> {
    static T* value = 0;
};
template<typename T>
struct TDefault<T&> {
    static T& value = TDefault<T>::value;
};
template<>
struct TDefault<void*> {
    //static const int value = 0; //static const void* value = 0: invalid in-class initialization of static data member of non-integral type 'const void*'
    enum { value = 0};
};
#define DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, ARG_T, ARG_T_V, ARG_V) \
    R name ARG_T_V { \
        typedef R (*api_t) ARG_T; \
        static api_t api = (api_t)dll::instance().resolve(sym); \
        if (!api) { \
            fprintf(stderr, "resolve symbol '%s' failed!\n", sym); \
            return (R)TDefault<R>::value; \
        } \
        return api ARG_V; \
    }

} //namespace DllAPI

#endif //DLLAPI_H

