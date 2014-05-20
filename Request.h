#ifndef _PLATER_REQUEST_H
#define _PLATER_REQUEST_H

#include <map>
#include <iostream>
#include <vector>
#include "Part.h"
#include "Solution.h"
#include "PlacedPart.h"

#define REQUEST_STL 0
#define REQUEST_PPM 1

namespace Plater
{
    class Request
    {
        public:
            Request();
            virtual ~Request();

            void readFromStdin();
            void process();
            PlacedPart *getNextPart();

            int mode;
            float precision;
            float delta, deltaR;
            float spacing;
            double plateWidth, plateHeight;
            std::string pattern;
            void writeFiles(Solution *solution);
            void writeSTL(Plate *plate, const char *filename);
            void writePpm(Plate *plate, const char *filename);
        
            std::map<std::string, int> quantities;
            std::map<std::string, Part*> parts;
    
            std::string readString();
            double readFloat();
            int readInt();
    };
}

#endif
