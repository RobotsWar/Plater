#ifndef _PLATER_PLACED_PART
#define _PLATER_PLACED_PART

#include "Part.h"

namespace Plater
{
    class PlacedPart
    {
        public:
            PlacedPart();
            virtual ~PlacedPart();

            void setPart(Part *part);
            void setOffset(float x, float y);
            void setRotation(float r);
            Part *getPart();
            float getX();
            float getY();
            
            float getGX();
            float getGY();

            Bitmap *getBmp();
            Bitmap *generateBitmap();

        protected:
            Bitmap *bmp;
            Part *part;
            float x, y;
            float rotation;
            bool dirty;
    };
}

#endif
