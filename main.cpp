//#include "SDL3/SDL_events.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cstdint>
#include <iostream>

int main(int argc, char* argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("", 520, 340, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }

    while(1)
    {
        SDL_PollEvent(&event);
        // event.type is Uint32
        if(event.type == SDL_EVENT_QUIT) // user-requested quit enum (SDL_EventType value = 256)
        {
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }


    // cleanup -> TODO: are the first two needed?
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
