#ifndef _PLATER_TRIANGLE_H
#define _PLATER_TRIANGLE_H

#include "floatpoint.h"

namespace Plater
{
    class Triangle
    {
        public:
            Triangle(FPoint2 A, FPoint2 B, FPoint2 C);

            bool contains(double x, double y);
            bool contains(const FPoint2 &p);

        protected:
            FPoint2 A, B, C;
            FPoint2 AB, BC, CA;
            FPoint2 nAB, nBC, nCA;
    };
}

#endif
