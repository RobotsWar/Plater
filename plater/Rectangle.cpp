#include "Rectangle.h"

namespace Plater
{
    Rectangle::Rectangle(double x1, double y1, double x2, double y2)
        : x1(x1), y1(y1), x2(x2), y2(y2)
    {
    }

    double Rectangle::getX1()
    {
        return x1;
    }

    double Rectangle::getY1()
    {
        return y1;
    }

    double Rectangle::getX2()
    {
        return x2;
    }

    double Rectangle::getY2()
    {
        return y2;
    }

    bool Rectangle::overlaps(const Rectangle &other)
    {
        return (x1 < other.x2 && x2 > other.x1)
            && (y1 < other.y2 && y2 > other.y1);
    }
}
