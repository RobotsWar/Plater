#ifndef _LINES_H
#define _LINES_H

#include <vector>
#include "Line.h"
#include "Bitmap.h"

namespace Plater
{
    class Lines
    {
        public:
            Lines();
            void addLine(Line line);
            void merge(const Lines &other);
			Bitmap *pixelize(float precision, float dilatation);
            FPoint2 min();
            FPoint2 max();

            std::vector<float> intersections(const Line &other);

            std::vector<Line> lines;
    };
}

#endif
