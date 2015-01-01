#ifndef _PLATER_REQUEST_H
#define _PLATER_REQUEST_H

#include <map>
#include <sstream>
#include <iostream>
#include <vector>
#include "Part.h"
#include "Solution.h"
#include "PlacedPart.h"

// Output modes
#define REQUEST_STL 0
#define REQUEST_PPM 1

// Explore different sortings
#define REQUEST_SINGLE_SORT      0
#define REQUEST_MULTIPLE_SORTS   1

namespace Plater
{
    class Request
    {
        public:
            Request();
            virtual ~Request();

            // Set the plate dimension
            void setPlateSize(float w, float h);

            // Read plater.conf
            void readFromStdin();
            void readFromFile(std::string filename);
            void readPartsFromString(std::string parts);
            void process();

            // Plate mode (rectangular or circular)
            int plateMode;
            // Plates dimensions
            double plateWidth, plateHeight;
            double plateDiameter;
            // Number of random iterations
            int randomIterations;
            // Output mode (STL or PPM)
            int mode;
            // Sort mode (simple or multiple)
            int sortMode;
            // Precision 
            float precision;
            // Brute-force deltas
            float delta, deltaR;
            // Parts spacing
            float spacing;
            // Output file pattern
            std::string pattern;

            void writeFiles(Solution *solution);
            void writeSTL(Plate *plate, const char *filename);
            void writePpm(Plate *plate, const char *filename);
        
            std::map<std::string, int> quantities;
            std::map<std::string, Part*> parts;
    
            // Request error
            bool hasError;
            std::string error;

            // Cancel the running request
            bool cancel;
            // Number of plates
            int plates;
            std::vector<std::string> generatedFiles;

            // Stats & solution
            int placersCount, placerCurrent;
            Solution *solution;

            // Number of threads
            unsigned int nbThreads;

        protected:
            void addPart(std::string filename, int quantity, std::string orientation);
            std::vector<std::string> getChunks(string line);
            void readParts();
            std::string readLine();
            std::istream *stream;
    };
}

#endif
