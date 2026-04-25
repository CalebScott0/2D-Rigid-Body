#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <iostream>
#include "Circle.hpp"
#include "SDL3/SDL_video.h"

const float RADIUS = 40.0f; // pixels

const float TIME_STEP = 1.0f / 60.0f; // seconds

const float ACCEL = 981.0f; // pixels / seconds^2

// given a circle c of radius r,
// draw a circle consisting of line connected points
void drawCircle(const Circle& c);

int main(int argc, char *argv[])
{
    // SDL structs
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    // attempt initialize SDL library subsystem video
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "ERROR: " << SDL_GetError() << std::endl;
        return 0;
    }

    // 1000 window width, 900 window height
    if(!SDL_CreateWindowAndRenderer("Window", 1000, 800, 0, &window, &renderer))
    {
        std::cout << "ERROR: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    // init our circle
    float xVelocity = 200.0f; // pixels / second
    Circle circle1(500.0f, 100.0f, RADIUS, 0.0f, xVelocity);

    int height;
    int width;
    bool done = false;

    // sim loop
    while(!done)
    {
        // poll all events and handle quit
        while(SDL_PollEvent(&event))
        {
            // types are enum, .type is uint32
            if(event.type == SDL_EVENT_QUIT)
            {
                done = true;
            }
        }

        SDL_GetWindowSize(window, &width, &height);

        // update state (euler method)
        // time step fixed at 1/60 second
        circle1.velocity.y += TIME_STEP * ACCEL;
        circle1.center.y += TIME_STEP * circle1.velocity.y;
        circle1.center.x += TIME_STEP * circle1.velocity.x;

        // check collision bottom
        if(circle1.center.y + RADIUS >= height)
        {
            // fix position slightly to avoid being stuck at bottom
            circle1.center.y = height - RADIUS;
            // velocity = -velocity to return up
            circle1.velocity.y *= -1.0f;
        }

        // render draws over whatever is already rendered
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // now set render color for points
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        drawCircle(circle1);
        SDL_RenderPresent(renderer);
    }


    return 0;
}

void drawCircle(const Circle& c)
{
    // how to determine #points based off of radius?
    // for i in numpoints,
    // we just want to find the theta offset, so 2.0f * M_PI * (current point # / total points #)
}
