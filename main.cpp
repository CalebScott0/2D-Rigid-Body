#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cmath>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <thread>
#include <chrono>
#include "Circle.hpp"

const float RADIUS = 40.0f; // pixels

const float TIME_STEP_60FPS = 1.0f / 60.0f; // seconds
const float TIME_STEP_120FPS = 1.0f / 120.0f; // seconds

const float ACCEL = 981.0f; // pixels / seconds^2

// given a circle c of radius r,
// draw a circle consisting of line connected points
void drawCircle(const Circle& c, SDL_Renderer* renderer);

int main(int argc, char *argv[])
{
    // default 60fps with no flag
    float time_step = TIME_STEP_60FPS;
    for(int i = 0; i < argc; ++i)
    {
        if(strcmp(argv[i], "--fps=120") == 0)
        {
            time_step = TIME_STEP_120FPS;
        }
    }

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
    float x_velocity = 200.0f; // pixels / second
    Circle circle1(500.0f - (4.0f * RADIUS), 100.0f, RADIUS, -x_velocity, 0.0f);
    Circle circle2(500.0f, 400.0f, RADIUS, 1.7f * x_velocity, 0.0f);

    // now make it for an arbitrary amount with a slider (pairs)
    // AND RANDOMIZE STARTING LOCATIONS (not touching)
    // next step is to add mass
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
        float dist_x = circles[0].center.x - circles[1].center.x;
        float dist_y = circles[0].center.y - circles[1].center.y;
        float dist = std::sqrt(dist_x*dist_x + dist_y * dist_y);

        // object collision
        if(dist < (2.0f * RADIUS))
        {
            // TODO: Fix these variable names lmaooooo
            
            // components of unit normal vector between objects' centers
            float norm_x = dist_x / dist;
            float norm_y = dist_y / dist;
            
            // components of unit tangent vector between the objects where the unit tangent is (-norm_y, norm_x)
            // Orthogonal (orthonormal) to the unit normal vector
            float tangX = -norm_y;
            float tangY = norm_x;

            // project the velocity vectors onto the unit normal and
            // unit tangent vectors by taking the dot product of the velocity vectors with the unit normal and
            // unit tangent vectors
            // Let v1_normal be the scalar (plain number, not a vector) velocity of object 1 in
            // the normal direction. Let v1_tangent be the scalar velocity of object 1 in the tangential direction.
            // Similarly, let v2_normal and v2_tangent be for object 2.
            // EQUAL MASS
            float v1_normal = norm_x * circles[0].velocity.x + norm_y * circles[0].velocity.y;
            float v2_normal = norm_x * circles[1].velocity.x + norm_y * circles[1].velocity.y;

            float v1_tangent = tangX * circles[0].velocity.x + tangY * circles[0].velocity.y;
            float v2_tangent = tangX * circles[1].velocity.x + tangY * circles[1].velocity.y;

            // update normal velocities after collision (follows new v1_normal` = v2_normal, new V1 normal = v2_normal dot unit normal
            float new_v1_normal_X = v2_normal * norm_x;
            float new_v1_normal_Y = v2_normal * norm_y;

            float new_v2_normal_X = v1_normal * norm_x;
            float new_v2_normal_Y = v1_normal * norm_y;

            // updated tangent velocities after collision (no force in the tangential direction, same scalar after collision)
            float new_v1_tangent_X = v1_tangent * tangX;
            float new_v1_tangent_Y = v1_tangent * tangY;

            float new_v2_tangent_X = v2_tangent * tangX;
            float new_v2_tangent_Y = v2_tangent * tangY;

            // now just add the new normal and tangent vectors together 
            circles[0].velocity.x = 0.9f * (new_v1_normal_X + new_v1_tangent_X);
            circles[0].velocity.y = 0.9f * (new_v1_normal_Y + new_v1_tangent_Y);

            circles[1].velocity.x = 0.9f * (new_v2_normal_X + new_v2_tangent_X);
            circles[1].velocity.y = 0.9f * (new_v2_normal_Y + new_v2_tangent_Y);
        }

        // update state (euler method)
        // time step fixed at 1/60 second
        for(Circle& c : circles)
        {
            c.velocity.y += time_step * ACCEL;
            c.center.y += time_step * c.velocity.y;
            c.center.x += time_step * c.velocity.x;


            // check collision bottom
            if(c.center.y + RADIUS >= height - 0.55)
            {
                // fix position slightly to avoid being stuck at bottom
                c.center.y = height - RADIUS - 0.55;
                // velocity = -velocity to return up
                c.velocity.y *= -0.85f;
            }
            if(c.center.x + RADIUS >= width)
            {
                c.center.x = width - RADIUS;
                c.velocity.x *= -0.78f;
            }
            else if(c.center.x - RADIUS <= 0)
            {
           
                c.center.x = RADIUS;
                c.velocity.x *= -0.78f;
            }


            drawCircle(c, renderer);
            // match frame rate

        }
    // TODO: add these options into a makefile with run commands
    // // also handle better?
    //   like in a man / help layout
    
    if(time_step == TIME_STEP_120FPS)
    {
        // ~8ms = 120fps (cap loop)
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
    else
    {
        // ~16ms = 60fps (cap loop)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
            
    SDL_RenderPresent(renderer);
    }


    return 0;
}

void drawCircle(const Circle& c, SDL_Renderer* renderer)
{
    //
    // Determine N : number of points 
    // - Small angle approx for starting distance (pixels) between each point
    // - sin(theta) = y/radius, where y is the vertical pixel height between points
    // - and theta is approximately = 2 * PI / N (if each theta is identical)
    //
    // to find the current theta offset from 0, theta = 2.0f * M_PI * (current point # / total points #)
    float y = 4.4; 
    // At most N = 255, ideally N <= ~ 64
    std::uint8_t N = 2.0f * M_PI * RADIUS / y;

    SDL_FPoint points[N + 1];

    for(auto i = 0; i <= N; ++i)
    {
        // TODO: see if you can smooth out the ratios (i/N) to make circles smoother?
        //  --- LOG HOW MUCH THE THETAS DIFFER
        points[i].x = c.center.x + RADIUS * std::cos(2.0f * M_PI * i / N);
        points[i].y = c.center.y + RADIUS * std::sin(2.0f * M_PI * i / N);
    }
    // wrap the last point back to the first
    points[N].x = points[0].x;
    points[N].y = points[0].y;

    SDL_RenderLines(renderer, points, N + 1);
}
