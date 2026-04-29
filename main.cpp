#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cmath>
#include <cstdint>
#include <iostream>
#include <thread>
#include <chrono>
#include "Circle.hpp"

const float RADIUS = 40.0f; // pixels

const float TIME_STEP = 1.0f / 60.0f; // seconds

const float ACCEL = 981.0f; // pixels / seconds^2

// given a circle c of radius r,
// draw a circle consisting of line connected points
void drawCircle(const Circle& c, SDL_Renderer* renderer);

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
    if(!SDL_CreateWindowAndRenderer("Window", 1000, 800, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        std::cout << "ERROR: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 0;
    }

    // init our circle
    float xVelocity = 200.0f; // pixels / second
    Circle circle1(500.0f - (4.0f * RADIUS), 100.0f, RADIUS, -xVelocity, 0.0f);
    Circle circle2(500.0f, 100.0f, RADIUS, xVelocity, 0.0f);

    int numCircle = 2;
    Circle circles[2] { circle1, circle2 };

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

        // render draws over whatever is already rendered
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        //SDL_RenderClear(renderer);
        // now set render color for points
        SDL_RenderClear(renderer);
        //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_SetRenderDrawColor(renderer, 0x27, 0xF5, 0x3C, 0xFF);

        // TODO: must test all pairs with arbitrary number once full
        
        // 2D Elastic Collisions without Trigonometry:
        // https://imada.sdu.dk/~rolf/Edu/DM815/E10/2dcollisions.pdf

        // distance components
        float distX = circles[0].center.x - circles[1].center.x;
        float distY = circles[0].center.y - circles[1].center.y;
        float dist = std::sqrt(distX*distX + distY * distY);

        // components of unit normal vector between objects' centers
        float normX = distX / dist;
        float normY = distY / dist;
        
        // components of unit tangent vector between the objects where the unit tangent is (-normY, normX)
        // Orthogonal (orthonormal) to the unit normal vector
        float tangX = -normY;
        float tangY = normX;

        // update state (euler method)
        // time step fixed at 1/60 second
        for(Circle& c : circles)
        {
            c.velocity.y += TIME_STEP * ACCEL;
            c.center.y += TIME_STEP * c.velocity.y;
            c.center.x += TIME_STEP * c.velocity.x;


            // check collision bottom
            if(c.center.y + RADIUS >= height)
            {
                // fix position slightly to avoid being stuck at bottom
                c.center.y = height - RADIUS;
                // velocity = -velocity to return up
                c.velocity.y *= -0.9f;
            }
            if(c.center.x + RADIUS >= width)
            {
                c.center.x = width - RADIUS;
                c.velocity.x *= -0.9f;
            }
            else if(c.center.x - RADIUS <= 0)
            {
           
                c.center.x = RADIUS;
                c.velocity.x *= -0.9f;
            }

            

            if(dist < (2.0f * RADIUS))
            {
                std::cout << "HIT" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                return 0;
                // handle velocity components
            }

            drawCircle(c, renderer);
            // match frame rate

        }
    // ~16ms = 60fps (cap loop)
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    SDL_RenderPresent(renderer);
    }

    return 0;
}

void drawCircle(const Circle& c, SDL_Renderer* renderer)
{
    // to find the theta offset, so 2.0f * M_PI * (current point # / total points #)
    //
    // Determine N : number of points 
    // Small angle approx starting distance (pixels) between each point
    // sin(theta) = y/radius, where y is the vertical pixel height between points
    float y = 4.4; 
    std::uint16_t N = 2.0f * M_PI * RADIUS / y;

    SDL_FPoint points[N + 1];

    for(auto i = 0; i <= N; ++i)
    {
        points[i].y = c.center.y + RADIUS * std::sin(2.0f * M_PI * i / N);
        points[i].x = c.center.x + RADIUS * std::cos(2.0f * M_PI * i / N);
    }
    points[N].y = points[0].y;
    points[N].x = points[0].x;

    SDL_RenderLines(renderer, points, N + 1);
}
