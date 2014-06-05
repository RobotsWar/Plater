#ifndef _PLATER_PLATE_H
#define _PLATER_PLATE_H

#include "Part.h"
#include "PlacedPart.h"

#define PLATE_MODE_RECTANGLE    0
#define PLATE_MODE_CIRCLE       1

namespace Plater
{
    class Plate
    {
        public:
            Plate(float width, float height, float diameter, int mode, float precision);
            virtual ~Plate();

            void place(PlacedPart *placedPart);
            bool canPlace(PlacedPart *placedPart);
            int countParts();
            Model createModel();

            float width, height;
            float diameter;
            int mode;
            float precision;
            Bitmap *bmp;
            vector<PlacedPart*> parts;
    };
}

#endif
