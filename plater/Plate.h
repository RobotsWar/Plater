#ifndef _PLATER_PLATE_H
#define _PLATER_PLATE_H

#include "Part.h"
#include "PlacedPart.h"

namespace Plater
{
    class Plate
    {
        public:
            Plate(float width, float height, float precision);
            virtual ~Plate();

            void place(PlacedPart *placedPart);
            bool canPlace(PlacedPart *placedPart);
            int countParts();
            Model createModel();

            float width, height;
            float precision;
            Bitmap *bmp;
            vector<PlacedPart*> parts;
    };
}

#endif
