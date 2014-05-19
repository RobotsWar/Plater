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
            void setRotation(int r);
            Part *getPart();
            float getX();
            float getY();

            float getSurface() const;
            float getGX();
            float getGY();

            Bitmap *getBmp();

        protected:
            Part *part;
            float x, y;
            int rotation;
    };
}

#endif
