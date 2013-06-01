#ifndef DLLAPI_SDL_H
#define DLLAPI_SDL_H

namespace DllAPI {
namespace SDL {

//TODO why include SDL/SDL.h will cause link error? otherwise, the other line are not required
//#include "SDL/SDL.h"

/*
 *if include SDL/SDL.h is ok, the code below is not requred
 *copy from SDL headers without any change
 */
#include "SDL/SDL_stdinc.h"
#include "SDL/SDL_events.h"
#include "SDL/SDL_video.h"
#define SDL_INIT_VIDEO		0x00000020

extern DECLSPEC int SDLCALL SDL_Init(Uint32 flags);
extern DECLSPEC void SDLCALL SDL_Quit(void);
extern DECLSPEC int SDLCALL SDL_PollEvent(SDL_Event *event);


extern DECLSPEC SDL_Surface * SDLCALL SDL_SetVideoMode
            (int width, int height, int bpp, Uint32 flags);
}
extern DECLSPEC void SDLCALL SDL_WM_SetCaption(const char *title, const char *icon);

}

#endif // DLLAPI_SDL_H
