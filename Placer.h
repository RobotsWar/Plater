#ifndef _PLATER_PLACER_H
#define _PLATER_PLACER_H

#include <map>
#include "Request.h"
#include "Plate.h"
#include "PlacedPart.h"
#include "Solution.h"

#define PLACER_SORT_SURFACE_INC 0
#define PLACER_SORT_SURFACE_DEC 1
#define PLACER_SORT_DENSITY_INC 2
#define PLACER_SORT_DENSITY_DEC 3
#define PLACER_SORT_SHUFFLE     4

#define PLACER_GRAVITY_YX       0
#define PLACER_GRAVITY_XY       1
#define PLACER_GRAVITY_EQ       2

namespace Plater
{
    class Placer
    {
        public:
            Placer(Request *request);

            void sortParts(int sortType);
            void setGravityMode(int gravityMode);
            void setRotateDirection(int direction);

            PlacedPart *getNextPart();
            Solution *place();

        protected:
            int rotateDirection;
            std::map<Plate *, std::map<std::string, bool> > cache;
            float xCoef, yCoef;
            vector<PlacedPart *> parts;
            Request *request;

            bool placePart(Plate *plate, PlacedPart *part);
    };
}

#endif
