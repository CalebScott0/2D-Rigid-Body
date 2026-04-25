#ifndef CIRCLE_H
#define CIRCLE_H

template<typename T>
struct Vec2
{
    T x;
    T y;

    Vec2(T _x, T _y)
        : x(_x),
          y(_y)
    { /* empty */ }
};

struct Circle
{
    // Circle as a shape only needs radius to construct geometrically
    float radius; 

    // Circle body has a Vec2 center position
    Vec2<float> center;

    Vec2<float> velocity; // pixels / sec


    Circle(float _x, float _y, float _r, float _vX, float _vY)
        : center(_x, _y),
          radius(_r),
          velocity(_vX, _vY)
    { 
        //empty
    }

    //Circle(float _x, float _y, float _r, float _vX, float _vY)
        //: center(_x, _y),
        //radius(_r)
    //{ /* empty */ }
};


#endif
