#include "dllapi/SDL/SDL.h"
using namespace DllAPI::SDL;

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
