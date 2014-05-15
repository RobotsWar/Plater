#ifndef _PLATER_RECTANGLE_H
#define _PLATER_RECTANGLE_H

namespace Plater
{
    class Rectangle
    {
        public:
            Rectangle(double x1, double y1, double x2, double y2);

            double getX1();
            double getY1();
            double getX2();
            double getY2();

            /**
             * Does this rectangle overlaps another one?
             */
            bool overlaps(const Rectangle &other);

        protected:
            double x1, y1, x2, y2;
    };
}

#endif
