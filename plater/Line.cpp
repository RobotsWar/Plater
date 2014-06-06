#include <math.h>
#include <stdio.h>
#include "Line.h"

namespace Plater
{
    Line::Line(float x_, float y_, float dx_, float dy_)
        : x(x_), y(y_), dx(dx_), dy(dy_)
    {
    }
            
    Line::Line(const Line &other)
        : x(other.x), y(other.y), dx(other.dx), dy(other.dy)
    {
    }
        
    bool Line::intersections(const Line &other, float *a, float *b) const
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
            
    FPoint2 Line::min()
    {
        FPoint2 pt;
        pt.x = (dx < 0) ? (x + dx) : x;
        pt.y = (dy < 0) ? (y + dy) : y;
        return pt;
    }

    FPoint2 Line::max()
    {
        FPoint2 pt;
        pt.x = (dx > 0) ? (x + dx) : x;
        pt.y = (dy > 0) ? (y + dy) : y;
        return pt;
    }
}
