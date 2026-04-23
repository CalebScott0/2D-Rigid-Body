#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <chrono>

#include <iostream>

#include "Circle.hpp"

void drawCircle(SDL_Renderer*, const Circle& body);

const float GRAVITY_ACCEL = 9.80665;
const int NUM_POINTS = 1000;
const float RADIUS = 40.0f;

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

    // move this to header file somewhere later but for now just get one circle going
    float velocityY = 0.0;

    while(1) 
    {
        SDL_PollEvent(&event);
        // event.type is Uint32
        // user-requested quit enum (SDL_EventType value = 256)
        if(event.type == SDL_EVENT_QUIT)
        {
            break;
        }

        // TODO: quick readover of C++ 11 book on the std::chrono library!!!
        // don't use auto unless you 100% understand what the datatype / class returned is

        // TODO: update state of body

        // rendering draws over whatever was drawn before
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_FPoint points[100];
        // current window size
        int width;
        int height;

        SDL_GetWindowSize(window, &width, &height);

        // SDL_ALPHA_OPAQUE just a macro for 255 (max alpha)
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE); 
        
        int xCenter = width >> 1;
        int yCenter = height >> 1;

        Circle circle1(xCenter, yCenter + 50, RADIUS);
        drawCircle(renderer, circle1);

        SDL_RenderPresent(renderer);
    }


    // cleanup -> TODO: are the first two needed?
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawCircle(SDL_Renderer* renderer, const Circle& body) // TODO: Expand to have x and y coordinates
{
    // TODO: eventually have a class that can handle the creation of a circle and its movement??
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE); // SDL_ALPHA_OPAQUE just a macro for 255 (max alpha)
    
    // SDL_FPoint
    // {
    //     float x;
    //     float y;
    // }

    SDL_FPoint points[NUM_POINTS];

    for(size_t i = 0; i < NUM_POINTS; ++i)
    {
        // theta calculated as ratio of current point to NUM_POINTS times max 2pi
        float theta = 2.0f * M_PI * i / NUM_POINTS;

        points[i].x = body.center.x + body.radius * std::cos(theta);
        points[i].y = body.center.y + body.radius * std::sin(theta);
    }

    SDL_RenderPoints(renderer, points, sizeof(points) / sizeof(SDL_FPoint));
}
