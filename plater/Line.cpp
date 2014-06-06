#include <math.h>
#include <stdio.h>
#include "Line.h"

namespace Plater
{
    Line::Line(float x_, float y_, float dx_, float dy_)
        : x(x_), y(y_), dx(dx_), dy(dy_)
    {
    }
        
    bool Line::intersections(const Line &other, float *a, float *b)
    {
        float det = dy*other.dx - dx*other.dy;

        if (det < 0.0001 && det > -0.0001) {
            return false;
        }

        float X = other.x - x;
        float Y = other.y - y;

        if (a != NULL) *a = (-other.dy*X + other.dx*Y)/det;
        if (b != NULL) *b = (-dy*X + dx*Y)/det;

        return true;
    }
}
