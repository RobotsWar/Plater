#ifndef _PLATER_PLACER_H
#define _PLATER_PLACER_H

#include <thread>
#include <map>
#include "Request.h"
#include "Plate.h"
#include "PlacedPart.h"
#include "Solution.h"

#define PLACER_SORT_SURFACE_DEC 0
#define PLACER_SORT_SURFACE_INC 1
#define PLACER_SORT_SHUFFLE     2

#define PLACER_GRAVITY_YX       0
#define PLACER_GRAVITY_XY       1
#define PLACER_GRAVITY_EQ       2

namespace Plater
{
    class Placer
    {
        public:
            Placer(Request *request);
            virtual ~Placer();

            void sortParts(int sortType);
            void setGravityMode(int gravityMode);
            void setRotateDirection(int direction);
            void setRotateOffset(int offset);

            PlacedPart *getNextPart();
            Solution *place();
            void placeThreaded();
            Solution *solution;
            
        protected:
            std::thread *myThread;
            int rotateOffset;
            int rotateDirection;
            std::map<Plate *, std::map<std::string, bool> > cache;
            float xCoef, yCoef;
            vector<PlacedPart *> parts;
            Request *request;

            bool placePart(Plate *plate, PlacedPart *part);
    };
}

#endif
