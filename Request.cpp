#include <iostream>
#include "Request.h"
#include "Placer.h"
#include "Plate.h"
#include "Solution.h"
#include "log.h"

using namespace std;

namespace Plater
{
    Request::Request()
        : precision(500), delta(2000), deltaR(M_PI/2)
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
        cin >> data;
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

    void Request::readFromStdin()
    {
        _log("* Reading request\n");
        plateWidth = readFloat()*1000;
        plateHeight = readFloat()*1000;
        _log("- Plate size: %g x %g µm\n", plateWidth, plateHeight);

        while (!cin.eof()) {
            string filename = readString();
            int quantity = readInt();
            _log("- Loading %s (quantity %d)...\n", filename.c_str(), quantity);
            if (filename != "" && quantity != 0) {
                parts[filename] = new Part;
                parts[filename]->load(filename, precision, deltaR);
                quantities[filename] = quantity;
            }
        }
    }

    void Request::process()
    {
        Solution *solution = NULL;

        for (int sortMode=0; sortMode<PLACER_SORT_SHUFFLE+2; sortMode++) {
            for (int gravity=0; gravity<PLACER_GRAVITY_EQ; gravity++) {
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

        _log("* Solution\n");
        _log("- Plates: %d\n", solution->countPlates());
        Plate *plate = solution->getPlate(0);
        plate->bmp->toPpm();
        delete solution;
    }
}
