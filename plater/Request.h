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

            std::vector<std::string> getChunks(string line);
            void setPlateSize(float w, float h);
            void addPart(std::string filename, int quantity, std::string orientation);
            void readFromStdin();
            void readFromFile(std::string filename);
            void readPartsFromString(std::string parts);
            void readParts();
            void process();

            int plateMode;
            double plateWidth, plateHeight;
            double plateDiameter;
            int randomIterations;
            int mode;
            float precision;
            float delta, deltaR;
            float spacing;
            std::string pattern;
            void writeFiles(Solution *solution);
            void writeSTL(Plate *plate, const char *filename);
            void writePpm(Plate *plate, const char *filename);
        
            std::map<std::string, int> quantities;
            std::map<std::string, Part*> parts;
    
            std::string readLine();
            std::string readString();
            double readFloat();
            int readInt();

            std::istream *stream;

            bool hasError;
            std::string error;

            bool cancel;
            int plates;
            std::vector<std::string> generatedFiles;

            int placersCount, placerCurrent;
            Solution *solution;

            unsigned int nbThreads;
    };
}

#endif
