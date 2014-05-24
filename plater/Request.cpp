#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "util.h"
#include "Request.h"
#include "Placer.h"
#include "Plate.h"
#include "Solution.h"
#include "modelFile/modelFile.h"
#include "log.h"

using namespace std;

#if defined(_WIN32) || defined(_WIN64)
#define snprintf _snprintf
#endif

namespace Plater
{
    Request::Request()
        : 
        plateWidth(150000),
        plateHeight(150000),
        randomIterations(3),
        mode(REQUEST_STL), 
        precision(500),
        delta(2000),
        deltaR(M_PI/2),
        spacing(2000),
        pattern("plate_%03d"),
        cancel(false)
    {
    }

    Request::~Request()
    {
        for (auto part : parts) {
            delete part.second;
        }
    }
            
    std::string Request::readLine()
    {
        char buffer[4096];
        stream->getline(buffer, 4096);

        return string(buffer);
    }

    std::string Request::readString()
    {
        std::string data;
        *stream >> data;

        return data;
    }

    double Request::readFloat()
    {
        return atof(readString().c_str());
    }

    int Request::readInt()
    {
        return atoi(readString().c_str());
    }

    void Request::setPlateSize(float w, float h)
    {
        plateWidth = w*1000;
        plateHeight = h*1000;
    }

    void Request::addPart(std::string filename, int quantity, string orientation)
    {
        if (!cancel && !hasError) {
            if (filename != "" && quantity != 0) {
                _log("- Loading %s (quantity %d, orientation %s)...\n", filename.c_str(), quantity, orientation.c_str());
                parts[filename] = new Part;
                parts[filename]->load(filename, precision, deltaR, spacing, orientation);
                quantities[filename] = quantity;

                // TODO: something smarter here
                if ((parts[filename]->width > plateWidth || parts[filename]->height > plateHeight)
                 && (parts[filename]->height > plateWidth || parts[filename]->width > plateHeight)) {
                    ostringstream oss;
                    oss << "Part " << filename << " is too big for the plate";
                    error = oss.str();
                    hasError = true;
                }
            }
        }
    }
            
    void Request::readPartsFromString(std::string parts)
    {
        istringstream s(parts);

        stream = &s;
        readParts();
    }

    void Request::readParts()
    {
        parts.clear();
        quantities.clear();

        hasError = false;
        while (!stream->eof()) {
            string line = readLine();
            if (line[0] != '#') {
                vector<string> chunks = splitWithEscape(line, ' ');
                if (chunks.size() > 0) {
                    string filename = chunks[0];
                    int quantity = 1;
                    string orientation = "bottom";

                    if (chunks.size() >= 2) quantity = atof(chunks[1].c_str());

                    if (chunks.size() >= 3) {
                        orientation = chunks[2];
                    }

                    try {
                        addPart(filename, quantity, orientation);
                    } catch (string error_) {
                        hasError = true;
                        error = error_;
                        return;
                    }
                }
            }
        }
    }
            
    void Request::readFromFile(std::string filename)
    {
        if (!chdirFile(filename)) {
            cerr << "! Can't go to the directory of " << filename << endl;
        }
        ifstream ifile(filename);

        if (!ifile) {
            cerr << "! Can't open configuration file " << filename << endl;
        } else {
            cerr << "* Reading from " << filename << endl;
            stream = &ifile;
            readParts();
        }
    }

    void Request::readFromStdin()
    {
        _log("* Reading request from stdin\n");
        stream = &cin;
        readParts();
    }
    
    void Request::writeSTL(Plate *plate, const char *filename)
    {
        SimpleModel model = plate->createModel();

        try {
            saveModelToFileBinary(filename, &model);
        } catch (string error_) {
            hasError = true;
            error = error_;
        }
    }

    void Request::writePpm(Plate *plate, const char *filename)
    {
        ofstream ofile(filename);
        if (ofile) {
            ofile << plate->bmp->toPpm();
            ofile.close();
        } else {
            ostringstream oss;
            oss << "Can't write to " << filename;
            error = oss.str();
            hasError = true;
            logError("Error: can't write to %s\n", filename);
        }
    }

    void Request::writeFiles(Solution *solution)
    {
        generatedFiles.clear();

        _log("* Exporting\n");
        switch (mode) {
            case REQUEST_PPM:
                pattern += ".ppm";
                break;
            case REQUEST_STL:
                pattern += ".stl";
                break;
        }

        char *buffer = new char[pattern.size()+64];
        for (int i=0; i<solution->countPlates(); i++) {
            Plate *plate = solution->getPlate(i);
            int plateNumber = i+1;
            snprintf(buffer, pattern.size()+63, pattern.c_str(), plateNumber);
            _log("- Exporting %s...\n", buffer);
            generatedFiles.push_back(string(buffer));

            switch (mode) {
                case REQUEST_STL:
                    writeSTL(plate, buffer);
                    break;
                case REQUEST_PPM:
                    writePpm(plate, buffer);
                    break;
            }
        }
        delete[] buffer;
    }

    void Request::process()
    {
        if (!cancel) {
            if (hasError) {
                cerr << "! Can't process: " << error << endl;
            } else {
                _log("- Plate size: %g x %g µm\n", plateWidth, plateHeight);
                Solution *solution = NULL;
                int rs = ceil(M_PI*2/deltaR);

                for (int rotateOffset=0; rotateOffset<2; rotateOffset++) {
                    for (int rotateDirection=0; rotateDirection<2; rotateDirection++) {
                        for (int sortMode=0; !cancel && sortMode<PLACER_SORT_SHUFFLE+randomIterations; sortMode++) {
                            for (int gravity=0; !cancel && gravity<PLACER_GRAVITY_EQ; gravity++) {
                                Placer placer(this);
                                placer.sortParts(sortMode);
                                placer.setGravityMode(gravity);
                                placer.setRotateDirection(rotateDirection);
                                placer.setRotateOffset(rotateOffset);

                                Solution *solutionTmp = placer.place();

                                if (solution == NULL || solutionTmp->score() < solution->score()) {
                                    solution = solutionTmp;
                                } else {
                                    delete solutionTmp;
                                }
                            }
                        }
                    }
                }

                if (!cancel) {
                    _log("* Solution\n");
                    _log("- Plates: %d\n", solution->countPlates());
                    _log("- Score: %g\n", solution->score());
                    writeFiles(solution);
                    plates = solution->countPlates();
                }
                delete solution;
            }
        }
    }
}
