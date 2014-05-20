#ifndef _PLATER_PLACER_H
#define _PLATER_PLACER_H

#include "Request.h"
#include "Plate.h"
#include "Solution.h"

#define PLACER_SORT_SURFACE_INC 0
#define PLACER_SORT_SURFACE_DEC 1
#define PLACER_SORT_SHUFFLE     3

namespace Plater
{
    class Placer
    {
        public:
            Placer(Request *request);

            void sortParts(int sortType);

            PlacedPart *getNextPart();
            Solution *place();

        protected:
            vector<PlacedPart *> parts;
            Request *request;
    };
}

#endif
