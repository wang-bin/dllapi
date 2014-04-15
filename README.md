# dllapi


This library helps you use C api in a shared library by dynamically loading instead of linking to it with minimal efforts.

For example, you have an SDL app linked to SDL library. If you want to let it support dynamically load SDL library and symbols without linking to it, you **JUST need 2 steps!** And the changes to the original source code is just include another header instead of SDL's header!

###Features

1. Let the existing code support dynamically loaded library with minimal changes.
2. The code is clear
3. Code once, use forever. And code easy!
4. Multiple library versions support at the same time
5. Maximum performance

###How To Use
The original source code using SDL:

>main.cpp


    #include <SDL/SDL.h>

    int main()
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
        SDL_WM_SetCaption("SDL Start", 0);
        SDL_Event event;
        bool gameRunning = true;
        while (gameRunning) {
            if (SDL_PollEvent(&event))
                gameRunning = event.type != SDL_QUIT;
        }
        SDL_Quit();
        return 0;
    }


####Step 1: create a header with dllapi

This step is quite easy. The header is **created once use forever!**

NOTE: you may simply include "SDL/SDL.h" in the namespace and do nothing else if that gives no compile error! 

Or you can include the api you want, including requred consts and macros. The api in the namespace `dllapi/SDL` are **JUST Copy from original headers**

Here is a simple SDL example:

>dllapi/SDL/SDL.h


    #ifndef DLLAPI_SDL_H
    #define DLLAPI_SDL_H

    namespace dllapi {
    namespace SDL {
    #include "SDL/SDL.h" //the original SDL header
    } //namespace SDL
    } //namespace dllapi
    #endif // DLLAPI_SDL_H


>dllapi/SDL/SDL.cpp (some code can be generated from  tools/mkapi)


    #include "dllapi_p.h"
    #include "dllapi.h"
    #include "dllapi/SDL/SDL.h"

    namespace dllapi {
    namespace SDL {

    //sdl lib names without prefix'lib' and suffix. it's SDL32.dll, or SDL.dll on windows.
    //DEFINE_DLL_INSTANCE_N("sdl", "SDL", "SDL32", "SDL-1.2", NULL)
    static char* sdl_names[] = { "SDL", "SDL32", "SDL-1.2", NULL };
    DEFINE_DLL_INSTANCE_V("sdl", sdl_names)
    DEFINE_DLLAPI_ARG(1, int, SDL_Init, Uint32)
    DEFINE_DLLAPI_ARG(2, void, SDL_WM_SetCaption, const char*, const char*)
    DEFINE_DLLAPI_ARG(1, int, SDL_PollEvent, SDL_Event*)
    DEFINE_DLLAPI_ARG(4, SDL_Surface*, SDL_SetVideoMode, int, int, int, Uint32)
    DEFINE_DLLAPI_ARG(0, void, SDL_Quit)

    } //namespace SDL
    } //namespace dllapi


`DEFINE_DLL_INSTANCE_N` defines the library names. "SDL" is SDL.dll or SDL32.dll in windows, libSDL.so in linux and libSDL.dylib in mac.

`DEFINE_DLLAPI_ARG` defines the api that declared in the header. Parameters are: number of arguments, return type, api name, parameters' types.

The new header(SDL.h) and source(SDL.cpp) can be used for other code contains SDL api.


####Step 2: just 2 lines in your source code and compile without linking

In this example, in main.cpp, you just replace `#include <SDL/SDL.h>` by `#include "dllapi/SDL/SDL.h"`, and add `using namespace dllapi::SDL;`. Then compile main.cpp without linking to SDL. Now all SDL functions are loaded dynamically.

>main.cpp(dynamically loaded symbols):


    #include "dllapi/SDL/SDL.h"
    using namespace dllapi::SDL;

    int main()
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
        SDL_WM_SetCaption("SDL Start", 0);
        SDL_Event event;
        bool gameRunning = true;
        while (gameRunning) {
            if (SDL_PollEvent(&event))
                gameRunning = event.type != SDL_QUIT;
        }
        SDL_Quit();
        return 0;
    }


###Performance

Comparing with linked symbols, calling this dynamically loaded symbol just cost **1 more** function call. So the performance is obviously very well.

###TODO

1. Write a tool to generate the header and source from the original ones automatically. Now it's `tools/mkapi` which based on clang


