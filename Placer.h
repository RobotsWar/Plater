#ifndef _PLATER_PLACER_H
#define _PLATER_PLACER_H

#include "Request.h"
#include "Plate.h"

#define PLACER_SORT_SURFACE_INC 0
#define PLACER_SORT_SURFACE_DEC 1

namespace Plater
{
    class Placer
    {
        public:
            Placer(Request *request);

            void sortParts(int sortType);

            PlacedPart *getNextPart();
            Plate *place();

        protected:
            vector<PlacedPart *> parts;
            Request *request;
    };
}

#endif
