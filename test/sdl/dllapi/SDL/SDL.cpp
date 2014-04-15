#include "dllapi_p.h"
#include "dllapi.h"
#include "SDL.h"

namespace dllapi {
namespace SDL {

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
