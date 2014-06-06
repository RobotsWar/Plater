#ifndef _LINE_H
#define _LINE_H

#include <unistd.h>

namespace Plater
{
    class Line
    {
        public:
            Line(float x, float y, float dx, float dy);

            bool intersections(const Line &other, float *a=NULL, float *b=NULL);

        protected:
            float x, y, dx, dy;
    };
}

#endif
