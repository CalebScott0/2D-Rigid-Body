#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>

#include "Circle.hpp"
#include "SDL3/SDL_timer.h"

void drawCircle(SDL_Renderer*, const Circle& body);

const float GRAVITY_ACCEL = 900.0f; // defined in pixels / secs^2
const int NUM_POINTS = 32;
const float RADIUS = 40.0f; 
const float DT = 1.0f / 120.f; // seconds

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

    //if (!SDL_CreateWindowAndRenderer("", 1000, 900, SDL_WINDOW_RESIZABLE, &window, &renderer))
    if (!SDL_CreateWindowAndRenderer("", 1000, 900, 0, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }

    int xCenter = 500;
    int yCenter = 100; 

    Circle circle1 (xCenter, yCenter, RADIUS, 200.0f, 0.0f);

    while(1) 
    {
        while(SDL_PollEvent(&event))
        {
            // event.type is Uint32
            // user-requested quit enum (SDL_EventType value = 256)
            if(event.type == SDL_EVENT_QUIT)
            {
                return 0;
            }
        }

        //SDL_FPoint points[100];
        
        int width;
        int height;

        SDL_GetWindowSize(window, &width, &height);

        // TODO: when you add another circle, draw out what the collision looks like with vectors on paper!
        //  i.e. it isn't just going oppositie directions, all components to be conserved!!


        // standard free fall kinematics eq (euler integration)
        circle1.velocity.y += GRAVITY_ACCEL * DT;
        circle1.center.y += circle1.velocity.y * DT;

        circle1.center.x += circle1.velocity.x * DT;

        // bottom collision
        if(circle1.center.y + RADIUS > height) 
        {
            // make sure the center is not past collision barrier
            circle1.center.y = height - RADIUS;
            // non conserative force -> restitution
            circle1.velocity.y *= -1.0f;
            //circle1.velocity.y *= -0.9f;
        }
        // side collision
        if(circle1.center.x + RADIUS > width || circle1.center.x - RADIUS < 0)
        {
            // TODO: fix this logic
            if(circle1.center.x - RADIUS < 0)
                circle1.center.x = RADIUS;
            //circle1.velocity.x *= -0.9f;
            circle1.velocity.x *= -1.0f;
        }

        // rendering draws over whatever was drawn before
        // SDL_ALPHA_OPAQUE just a macro for 255 (max alpha)
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE); 

        drawCircle(renderer, circle1);

        // pause thread for at least 16 milliseconds (60fps)
        // https://wiki.libsdl.org/SDL3/SDL_Delay
        // "This function waits a specified number of milliseconds before returning. 
        // It waits at least the specified time, but possibly longer due to OS scheduling."
        std::uint32_t d = 8; // approx 1/60 second in milliseconds
        SDL_Delay(d); // OS sleep functionality

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawCircle(SDL_Renderer* renderer, const Circle& body)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE); // SDL_ALPHA_OPAQUE just a macro for 255 (max alpha)
    
    // SDL_FPoint
    // {
    //     float x;
    //     float y;
    // }

    SDL_FPoint points[NUM_POINTS + 1];

    // define # points based on radius
    // where 2r = ... TODO:

    for(size_t i = 0; i < NUM_POINTS; ++i)
    {
        // theta calculated as ratio of current point to NUM_POINTS times max 2pi
        float theta = 2.0f * M_PI * i / NUM_POINTS;

        points[i].x = body.center.x + body.radius * std::cos(theta);
        points[i].y = body.center.y + body.radius * std::sin(theta);
    }

    // wrap points back around to connect for line
    points[NUM_POINTS] = points[0];

    SDL_RenderLines(renderer, points, NUM_POINTS+1);
}
