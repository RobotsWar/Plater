#include <iostream>
#include "Request.h"
#include "Placer.h"
#include "Plate.h"
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
        _log("- Plate size: %d x %d µm\n", plateWidth, plateHeight);

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
        Placer placer(this);
        placer.sortParts(2);
        Plate *plate = placer.place();
        plate->bmp->toPpm();
        delete plate;
    }
}
