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

#ifndef DLLAPI_GLOBAL_H
#define DLLAPI_GLOBAL_H

/*TODO: dllapi_version.h: _if __cpluspls namespace DLLAPI { enum Version {} }*/
#undef DLLAPI_VERSION

#define DLLAPI_VERSION_MAJOR 1	//((DLLAPI_VERSION & 0xff0000)>>16)
#define DLLAPI_VERSION_MINOR 0	//((DLLAPI_VERSION & 0xff00)>>8)
#define DLLAPI_VERSION_PATCH 0	//(DLLAPI_VERSION & 0xff)

#define DLLAPI_VERSION_CHK(major, minor, patch) \
    (((major&0xff)<<16) | ((minor&0xff)<<8) | (patch&0xff))

#define DLLAPI_VERSION DLLAPI_VERSION_CHK(DLLAPI_VERSION_MAJOR, DLLAPI_VERSION_MINOR, DLLAPI_VERSION_PATCH)

/*! Stringify \a x. */
#define _TOSTR(x)   #x
/*! Stringify \a x, perform macro expansion. */
#define TOSTR(x)  _TOSTR(x)

/* C++11 requires a space between literal and identifier */
#define DLLAPI_VERSION_STR		   TOSTR(DLLAPI_VERSION_MAJOR) "." TOSTR(DLLAPI_VERSION_MINOR) "." TOSTR(DLLAPI_VERSION_PATCH)
#define DLLAPI_VERSION_STR_LONG   DLLAPI_VERSION_STR "(" __DATE__ ", " __TIME__ ")"
static const char* const dllapi_version_string = DLLAPI_VERSION_STR_LONG;



#if defined(__ARMCC__) || defined(__CC_ARM)
#  define Q_CC_RVCT
#endif
#if defined(MSDOS) || defined(_MSDOS)
#  define Q_OS_MSDOS
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define Q_OS_WIN32
#  define Q_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(WINCE) || defined(_WIN32_WCE)
#    define Q_OS_WINCE
#  else
#    define Q_OS_WIN32
#  endif
#elif defined(__MWERKS__) && defined(__INTEL__)
#  define Q_OS_WIN32
#elif defined(hpux) || defined(__hpux)
#  define Q_OS_HPUX
#elif defined(__linux__) || defined(__linux)
#  define Q_OS_LINUX
#endif

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64) || defined(Q_OS_WINCE)
#  define Q_OS_WIN
#endif

/* VxWorks' DIAB toolchain has an additional EDG type C++ compiler
   (see __DCC__ above). This one is for C mode files (__EDG is not defined) */
#if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
/* 5.0 compiler or better
    'bool' is enabled by default but can be disabled using -features=nobool
    in which case _BOOL is not defined
        this is the default in 4.2 compatibility mode triggered by -compat=4 */
#  if __SUNPRO_CC >= 0x500
#    define QT_NO_TEMPLATE_TEMPLATE_PARAMETERS
/* see http://developers.sun.com/sunstudio/support/Ccompare.html */
#    if __SUNPRO_CC >= 0x590
#      define Q_ALIGNOF(type)   __alignof__(type)
#      define Q_TYPEOF(expr)    __typeof__(expr)
#      define Q_DECL_ALIGN(n)   __attribute__((__aligned__(n)))
#    endif
#    if __SUNPRO_CC >= 0x550
#      define Q_DECL_EXPORT     __global
#    endif
#  endif

/* CDS++ does not seem to define __EDG__ or __EDG according to Reliant
   documentation but nevertheless uses EDG conventions like _BOOL */
#elif defined(Q_OS_HPUX)
/* __HP_aCC was not defined in first aCC releases */
#  if defined(__HP_aCC) || __cplusplus >= 199707L
#    if __HP_aCC-0 < 060000
#      define Q_DECL_EXPORT     __declspec(dllexport)
#      define Q_DECL_IMPORT     __declspec(dllimport)
#    endif
#    if __HP_aCC-0 >= 061200
#      define Q_DECL_ALIGN(n) __attribute__((aligned(n)))
#    endif
#    if __HP_aCC-0 >= 062000
#      define Q_DECL_EXPORT     __attribute__((visibility("default")))
#      define Q_DECL_HIDDEN     __attribute__((visibility("hidden")))
#      define Q_DECL_IMPORT     Q_DECL_EXPORT
#    endif
#  endif

#elif defined(__WINSCW__) && !defined(Q_CC_NOKIAX86)
#  define Q_CC_NOKIAX86
#endif

#ifdef __GNUC__
#  define Q_LIKELY(expr)    __builtin_expect(!!(expr), 1)
#  define Q_UNLIKELY(expr)  __builtin_expect(!!(expr), 0)
#else
#  define Q_LIKELY(x) (x)
#  define Q_UNLIKELY(x) (x)
#endif

#if defined(Q_OS_LINUX) && defined(Q_CC_RVCT)
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#  define Q_DECL_HIDDEN     __attribute__((visibility("hidden")))
#endif

#ifndef Q_DECL_EXPORT
#  if defined(Q_OS_WIN) || defined(Q_CC_NOKIAX86) || defined(Q_CC_RVCT)
#    define Q_DECL_EXPORT __declspec(dllexport)
#  elif defined(QT_VISIBILITY_AVAILABLE)
#    define Q_DECL_EXPORT __attribute__((visibility("default")))
#    define Q_DECL_HIDDEN __attribute__((visibility("hidden")))
#  endif
#  ifndef Q_DECL_EXPORT
#    define Q_DECL_EXPORT
#  endif
#endif
#ifndef Q_DECL_IMPORT
#  if defined(Q_OS_WIN) || defined(Q_CC_NOKIAX86) || defined(Q_CC_RVCT)
#    define Q_DECL_IMPORT __declspec(dllimport)
#  else
#    define Q_DECL_IMPORT
#  endif
#endif
#ifndef Q_DECL_HIDDEN
#  define Q_DECL_HIDDEN
#endif


#if defined(BUILD_DLLAPI_LIB)
#  undef DLLAPI_EXPORT
#  define DLLAPI_EXPORT Q_DECL_EXPORT
#else
#  undef DLLAPI_EXPORT
#  define DLLAPI_EXPORT Q_DECL_IMPORT //only for vc?
#endif

#endif // DLLAPI_GLOBAL_H

