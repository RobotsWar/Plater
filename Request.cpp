#include <sstream>
#include <fstream>
#include <iostream>
#include "Request.h"
#include "Placer.h"
#include "Plate.h"
#include "Solution.h"
#include "modelFile/modelFile.h"
#include "log.h"

using namespace std;

namespace Plater
{
    Request::Request()
        : mode(REQUEST_STL), 
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

    void Request::addPart(std::string filename, int quantity)
    {
        if (!cancel && !hasError) {
            if (filename != "" && quantity != 0) {
                _log("- Loading %s (quantity %d)...\n", filename.c_str(), quantity);
                parts[filename] = new Part;
                parts[filename]->load(filename, precision, deltaR, spacing);
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
        hasError = false;
        while (!stream->eof()) {
            string filename = readString();
            int quantity = readInt();
            try {
                addPart(filename, quantity);
            } catch (string error_) {
                hasError = true;
                error = error_;
                return;
            }
        }
    }

    void Request::readFromStdin()
    {
        stream = &cin;

        _log("* Reading request from stdin\n");
        plateWidth = readFloat()*1000;
        plateHeight = readFloat()*1000;
        _log("- Plate size: %g x %g µm\n", plateWidth, plateHeight);

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
                cerr << "Can't process: " << error << endl;
            } else {
                Solution *solution = NULL;

                for (int sortMode=0; !cancel && sortMode<PLACER_SORT_SHUFFLE+2; sortMode++) {
                    for (int gravity=0; !cancel && gravity<PLACER_GRAVITY_EQ; gravity++) {
                        Placer placer(this);
                        placer.sortParts(sortMode);
                        placer.setGravityMode(gravity);

                        Solution *solutionTmp = placer.place();

                        if (solution == NULL || solutionTmp->score() < solution->score()) {
                            solution = solutionTmp;
                        } else {
                            delete solutionTmp;
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
