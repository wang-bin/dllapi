#include "dllapi_p.h"
#include "dllapi.h"
#include "SDL.h"

namespace DllAPI {
namespace SDL {

namespace {
struct dll_name_init {
    dll_name_init() {
        std::list<std::string> names;
        names.push_back("SDL32");
        addLibraryNames("SDL", names);
    }
} init;
}

DEFINE_DLL_INSTANCE("SDL")
DEFINE_DLLAPI_ARG(1, int, SDL_Init, Uint32)
DEFINE_DLLAPI_ARG(2, void, SDL_WM_SetCaption, const char*, const char*)
DEFINE_DLLAPI_ARG(1, int, SDL_PollEvent, SDL_Event*)
DEFINE_DLLAPI_ARG(4, SDL_Surface*, SDL_SetVideoMode, int, int, int, Uint32)
DEFINE_DLLAPI_ARG(0, void, SDL_Quit)

} //namespace SDL
} //namespace DllAPI
