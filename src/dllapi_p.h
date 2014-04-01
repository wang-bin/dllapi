#ifndef DLLAPI_P_H
#define DLLAPI_P_H

//THIS FILE IS NOT PUBLIC HEADER!
/*!
 * in dllapi/CL/cl.cpp, define the apis use macro
 *
 * in your application, use the api as usual, except that you should include headers like below:
 * #if CONFIG_DLLAPI_CL
 * #include <dllapi/CL/cl.h>
 * using namespace dllapi:OpenCL;
 * #else
 * #include <CL/cl.h>
 * #endif
 *
 * TODO: study COM api
 */

#include "dllapi_global.h"
#include <assert.h>
#include <stdio.h>
#include <string>

#if defined(_MSC_VER)
#pragma warning(disable:4098) //vc return void
#endif //_MSC_VER


#define DEBUG 1
#ifdef DBG
#undef DBG
#endif //DBG
#if DEBUG
#define DBG(fmt, ...) \
    printf("[%s] %s@%d: " fmt, __FILE__, DLLAPI_FUNC, __LINE__, ##__VA_ARGS__); \
    fflush(0);
#else
#define DBG(...)
#endif //DEBUG


#if defined(__GNUC__)
#  define DLLAPI_FUNC __PRETTY_FUNCTION__
#else
#   if defined(_MSC_VER)
#      define DLLAPI_FUNC __FUNCSIG__
#   else
#      define DLLAPI_FUNC __FUNCTION__
#   endif
#endif

namespace dllapi {


class DLLAPI_EXPORT DllObject {
public:
    DllObject(const std::string& name = "")
        : loaded(false)
    {
        if (!name.empty())
            setFileName(name);
    }
    ~DllObject() { unload(); }
    std::string errorString() const { return error;}
    std::string fileName() const { return file;}
    void setFileName(const std::string& name);
    bool isLoaded() const { return loaded; }
    bool load();
    bool unload();
    void* resolve(const std::string& symb);
private:
    bool _load(bool tryprefix);
    void* _reslove(const std::string& symb, bool again = false);
    bool loaded;
    void *handle;
    std::string file;
    std::string error;
};

/*
 *TODO: move load, unload, sDllMap, to dllapi_p.h(also map), keep this file empty?
 *if this is empty, then we don't need this. so we can move them here, and
 * this file is only used in xxx.cpp, e.g. dllapi/CL/cl.cpp
 * and dllapi/CL/cl.h is clean, only CL/cl.h content and dllapi::OpenCL namespace
 */
DLLAPI_EXPORT DllObject* library(const char* dllname); //TODO: return pair? it's already private

/********************************** The following code is only used in .cpp **************************************************/
//e.g. DEFINE_DLLAPI_SYM_ARG(3, cl_int, clGetPlatformIDs, cl_uint, cl_platform_id*, cl_uint*)
//fully expand. used by VC. VC will not expand __VA_ARGS__ but treats it as 1 parameter
#define EXPAND(expr) expr
/*!
 * N: number of arguments
 * R: return type
 * name: api name
 * ...: api arguments with only types
 * The symbol of the api is "name". Otherwise, use DEFINE_DLLAPI_SYM_ARGN instead.
 */
#define DEFINE_DLLAPI_ARG(N, R, name, ...) \
    EXPAND(DEFINE_DLLAPI_SYM_ARG##N(R, name, name, __VA_ARGS__))
//EXPAND(DEFINE_DLLAPI_SYM_ARG##N(R, name, #name, __VA_ARGS__))
#define DEFINE_DLLAPI_M_ARG(N, R, M, name, ...) \
    EXPAND(DEFINE_DLLAPI_M_SYM_ARG##N(R, M, name, name, __VA_ARGS__))
//EXPAND(DEFINE_DLLAPI_SYM_ARG##N(R, name, #name, __VA_ARGS__))

/*!
 * used by .cpp to define the api
 *  e.g. DEFINE_DLLAPI_SYM_ARG3(cl_int, clGetPlatformIDs, "clGetPlatformIDs", cl_uint, cl_platform_id*, cl_uint*)
 * sym: symbol of the api in library.
 */
#define DEFINE_DLLAPI_SYM_ARG0(R, name, sym, ...) DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, (), (), ())
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

#define DEFINE_DLLAPI_M_SYM_ARG0(R, M, name, sym, ...) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (), (), ())
#define DEFINE_DLLAPI_M_SYM_ARG1(R, M, name, sym, P1) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1), (P1 p1), (p1))
#define DEFINE_DLLAPI_M_SYM_ARG2(R, M, name, sym, P1, P2) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2), (P1 p1, P2 p2), (p1, p2))
#define DEFINE_DLLAPI_M_SYM_ARG3(R, M, name, sym, P1, P2, P3) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3), (P1 p1, P2 p2, P3 p3), (p1, p2, p3))
#define DEFINE_DLLAPI_M_SYM_ARG4(R, M, name, sym, P1, P2, P3, P4) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4), (P1 p1, P2 p2, P3 p3, P4 p4), (p1, p2, p3, p4))
#define DEFINE_DLLAPI_M_SYM_ARG5(R, M, name, sym, P1, P2, P3, P4, P5) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4, P5), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5), (p1, p2, p3, p4, p5))
#define DEFINE_DLLAPI_M_SYM_ARG6(R, M, name, sym, P1, P2, P3, P4, P5, P6) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4, P5, P6), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6), (p1, p2, p3, p4, p5, p6))
#define DEFINE_DLLAPI_M_SYM_ARG7(R, M, name, sym, P1, P2, P3, P4, P5, P6, P7) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4, P5, P6, P7), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7), (p1, p2, p3, p4, p5, p6, p7))
#define DEFINE_DLLAPI_M_SYM_ARG8(R, M, name, sym, P1, P2, P3, P4, P5, P6, P7, P8) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8), (p1, p2, p3, p4, p5, p6, p7, p8))
#define DEFINE_DLLAPI_M_SYM_ARG9(R, M, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9), (p1, p2, p3, p4, p5, p6, p7, p8, p9))
#define DEFINE_DLLAPI_M_SYM_ARG10(R, M, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10))
#define DEFINE_DLLAPI_M_SYM_ARG11(R, M, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11))
#define DEFINE_DLLAPI_M_SYM_ARG12(R, M, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12))
#define DEFINE_DLLAPI_M_SYM_ARG13(R, M, name, sym, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13) DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, (P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13), (P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12, P13 p13), (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13))



template<typename T>
struct IsVoid {
    static const int value = 0;
};
template<>
struct IsVoid<void> {
    static const int value = 1;
};

template<typename T>
struct Default {
    static const T value = T();
};
template<>
struct Default<void> {
    static const int value = 0;
};
template<typename T>
struct Default<T*> {
    static T* value;
};
template<typename T> T* Default<T*>::value = 0;

template<typename T>
struct Default<T&> {
    static T& value;
};
template<typename T> T& Default<T&>::value = Default<T>::value; //int*&, int&&
template<>
struct Default<void*> {
    //static const int value = 0; //static const void* value = 0: invalid in-class initialization of static data member of non-integral type 'const void*'
    enum { value = 0};
};
#define DEFINE_DLLAPI_SYM_ARG_T_V(R, name, sym, ARG_T, ARG_T_V, ARG_V) \
    R name ARG_T_V { \
        typedef R (*api_t) ARG_T; \
        static api_t api = (api_t)dll::instance().resolve(#sym); \
        if (Q_LIKELY(api)) \
            return api ARG_V; \
        fprintf(stderr, "resolve symbol '%s' failed!\n", #sym); \
        static int failed_to_resolve_symbol_##sym = 0; \
        assert(failed_to_resolve_symbol_##sym); \
        if (!IsVoid<R>::value) return (R)Default<R>::value; \
    }

#define DEFINE_DLLAPI_M_SYM_ARG_T_V(R, M, name, sym, ARG_T, ARG_T_V, ARG_V) \
    R M name ARG_T_V { \
        typedef R (*api_t) ARG_T; \
        static api_t api = (api_t)dll::instance().resolve(#sym); \
        if (Q_LIKELY(api)) \
            return api ARG_V; \
        fprintf(stderr, "resolve symbol '%s' failed!\n", #sym); \
        static int failed_to_resolve_symbol_##sym = 0; \
        assert(failed_to_resolve_symbol_##sym); \
        if (!IsVoid<R>::value) return (R)Default<R>::value; \
    }
//TODO: add multiple dll names support. e.g. in unix it's OpenGL, in windows is OpenGL32
/*
 * TODO: dllname used as id. e.g. on windows, 'OpenAL' may be 'OpenAL32.dll', we should add both OpenAL.dll and OpenAL32.dll. The dllid is also used 
 * Of cause, we can use platform macros to choose the dll names
 * DEFINE_DLL_INSTANCE_N("OpenAL"
 * #ifdef __WIN32
 * 	, "OpenAL32"
 * #else if ...
 */
/*!
 * \brief addLibraryNames
 * \param lib
 *  add a list of dll names for \a lib. must end with NULL!
 */
DLLAPI_EXPORT void addLibraryNames(const std::string& lib, ...);

DLLAPI_EXPORT void addLibraryNames(const std::string &lib, char** cnames);

#define DEFINE_DLL_INSTANCE_N(dllid, ...) \
    class dll { \
    public: \
        static dll& instance() { \
            static dll d; \
            return d; \
        } \
        void* resolve(const char* sym) { \
            DBG("%s (symbol: %s)\n", DLLAPI_FUNC, sym); \
            if (!mLoaded) \
                return 0; \
            return (void*)dllapi::library(dllid)->resolve(sym); \
        } \
    private: \
        dll() { \
            addLibraryNames(dllid, ##__VA_ARGS__); \
            mLoaded = testLoad(dllid); \
        } \
        ~dll() { unload(dllid); } \
        bool mLoaded; \
    };

// compiler may not support init list {a, b, c}
#define DEFINE_DLL_INSTANCE_V(dllid, names) \
    class dll { \
    public: \
        static dll& instance() { \
            static dll d; \
            return d; \
        } \
        void* resolve(const char* sym) { \
            DBG("%s (symbol: %s)\n", DLLAPI_FUNC, sym); \
            if (!mLoaded) \
                return 0; \
            return (void*)dllapi::library(dllid)->resolve(sym); \
        } \
    private: \
        dll() { \
            addLibraryNames(dllid, names); \
            mLoaded = testLoad(dllid); \
        } \
        ~dll() { unload(dllid); } \
        bool mLoaded; \
    };

#define DEFINE_DLL_INSTANCE(dllname) \
    class dll { \
    public: \
        static dll& instance() { \
            static dll d; \
            return d; \
        } \
        void* resolve(const char* sym) { \
            DBG("%s (symbol: %s)\n", DLLAPI_FUNC, sym); \
            if (!mLoaded) \
                return 0; \
            return (void*)dllapi::library(dllname)->resolve(sym); \
        } \
    private: \
        dll() { mLoaded = testLoad(dllname); } \
        ~dll() { unload(dllname); } \
        bool mLoaded; \
    };


} //namespace dllapi
#endif // DLLAPI_P_H
