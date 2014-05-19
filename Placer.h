#ifndef _PLATER_PLACER_H
#define _PLATER_PLACER_H

#include "Request.h"
#include "Plate.h"

namespace Plater
{
    class Placer
    {
        public:
            Placer(Request *request);

            PlacedPart *getNextPart();
            Plate *place();

        protected:
            vector<PlacedPart *> parts;
            Request *request;
    };
}

#endif
