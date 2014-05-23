#include "Rectangle.h"

namespace Plater
{
    Rectangle::Rectangle()
    {
    }

    Rectangle::Rectangle(float x1, float y1, float x2, float y2)
        : x1(x1), y1(y1), x2(x2), y2(y2)
    {
    }

    bool Rectangle::overlaps(const Rectangle &other)
    {
        return (x1 <= other.x2 && x2 >= other.x1)
            && (y1 <= other.y2 && y2 >= other.y1);
    }

    bool Rectangle::contains(float x, float y)
    {
        return (x1 <= x && x <= x2 && y1 <= y && y <= y2);
    }
}
