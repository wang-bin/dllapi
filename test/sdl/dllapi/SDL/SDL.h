#ifndef DLLAPI_SDL_H
#define DLLAPI_SDL_H

namespace dllapi {
namespace SDL {

// you can simply include "SDL/SDL.h" here and without any other code.
#include "SDL/SDL.h"
/*
 *copy from SDL headers without any change.
 */
/*
#include "SDL/SDL_stdinc.h"
#include "SDL/SDL_events.h"
#include "SDL/SDL_video.h"
#define SDL_INIT_VIDEO		0x00000020

extern DECLSPEC int SDLCALL SDL_Init(Uint32 flags);
extern DECLSPEC void SDLCALL SDL_Quit(void);
extern DECLSPEC int SDLCALL SDL_PollEvent(SDL_Event *event);
extern DECLSPEC SDL_Surface * SDLCALL SDL_SetVideoMode
            (int width, int height, int bpp, Uint32 flags);
extern DECLSPEC void SDLCALL SDL_WM_SetCaption(const char *title, const char *icon);
*/
} //namespace SDL
} //namespace dllapi

#endif // DLLAPI_SDL_H
