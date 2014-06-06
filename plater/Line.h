#ifndef _LINE_H
#define _LINE_H

#include <unistd.h>
#include "stl/Point3.h"

namespace Plater
{
    class Line
    {
        public:
            Line(float x, float y, float dx, float dy);
            Line(const Line &other);

            FPoint2 min();
            FPoint2 max();

            bool intersections(const Line &other, float *a=NULL, float *b=NULL) const;

        protected:
            float x, y, dx, dy;
    };
}

#endif
