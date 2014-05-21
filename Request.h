#ifndef _PLATER_REQUEST_H
#define _PLATER_REQUEST_H

#include <map>
#include <sstream>
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

            void setPlateSize(float w, float h);
            void addPart(std::string filename, int quantity);
            void readFromStdin();
            void readPartsFromString(std::string parts);
            void readParts();
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

            std::istream *stream;

            void fatalError(std::string message);
            bool hasError;
            std::string error;

            bool cancel;
            int plates;
    };
}

#endif
