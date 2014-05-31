#ifndef _PLATER_PLACED_PART
#define _PLATER_PLACED_PART

#include <string>
#include "Part.h"

namespace Plater
{
    class PlacedPart
    {
        public:
            PlacedPart();
            virtual ~PlacedPart();

            std::string getName();

            void setPart(Part *part);
            void setOffset(float x, float y);
            void setRotation(int r);
            Part *getPart();
            float getX();
            float getY();
            float getCenterX();
            float getCenterY();
            int getRotation();

            float getSurface() const;
            float getGDist() const;
            float getGX() const;
            float getGY() const;

            Bitmap *getBmp() const;

            Model createModel();

        protected:
            Part *part;
            float x, y;
            int rotation;
    };
}

#endif
