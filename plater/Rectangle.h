#ifndef _PLATER_RECTANGLE_H
#define _PLATER_RECTANGLE_H

namespace Plater
{
    class Rectangle
    {
        public:
            Rectangle();
            Rectangle(float x1, float y1, float x2, float y2);
            bool overlaps(const Rectangle &other);
            bool contains(float x, float y);
            float x1, y1, x2, y2;
    };
}

#endif
