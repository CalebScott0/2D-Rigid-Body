#ifndef CIRCLE_H
#define CIRCLE_H

// A 2D Vector, point / free vector
struct Vec2
{
    // pixel coordinates
    float x, y;
    
    // TODO: default constructor?
    Vec2(float _x, float _y)
        : x(_x),
          y(_y)
    {
        // empty
    }
};
// TODO: overload operators Vec2? 

struct Circle
{
    // location of object's center
    Vec2 center;
    // radius of circle
    float radius;
    // current linear velocity of  circle
    Vec2 velocity; // pixels / sec
                   
    Circle(float _xc, float _yc, float _r, float _xv, float _yv)
        : center(_xc, _yc),
          radius(_r),
          velocity(_xv, _yv)
    {
        // empty
    }
};

#endif
