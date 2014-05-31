#ifndef _PLATER_TRIANGLE_H
#define _PLATER_TRIANGLE_H

#include "stl/Point3.h"
#include "Rectangle.h"

namespace Plater
{
    class Triangle
    {
        public:
            Triangle();
            Triangle(FPoint2 A, FPoint2 B, FPoint2 C);
            void setPoints(FPoint2 A, FPoint2 B, FPoint2 C);

            bool contains(double x, double y);
            bool contains(const FPoint2 &p);
            bool contains(const Rectangle &rect);

            Rectangle box;
        protected:
            FPoint2 A, B, C;
            FPoint2 AB, BC, CA;
            FPoint2 nAB, nBC, nCA;
    };
}

#endif
