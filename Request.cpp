#include <iostream>
#include "Request.h"
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

    PlacedPart *Request::getNextPart() {
        for (auto part : quantities) {
            if (part.second > 0) {
                quantities[part.first] = quantities[part.first]-1;
                PlacedPart *placedPart = new PlacedPart;
                placedPart->setPart(parts[part.first]);
                return placedPart;
            }
        }

        return NULL;
    }

    void Request::process()
    {
        Plate plate(plateWidth, plateHeight, precision);

        _log("* Solver\n");
        while (PlacedPart *part = getNextPart()) {
            if (part == NULL) {
                break;
            }
            _log("- Trying to place %s...\n", part->getPart()->getFilename().c_str());
            float betterX=0, betterY=0, betterScore;
            int betterR=0;
            int rs = (M_PI*2/deltaR);
            bool found = false;
            for (int r=0; r<rs; r++) {
                part->setRotation(r);
                for (float x=0; x<plateWidth; x+=delta) {
                    for (float y=0; y<plateHeight; y+=delta) {
                        float gx = part->getGX()+x;
                        float gy = part->getGY()+y;
                        part->setOffset(x, y);

                        float score = gy*10+gx;

                        if (plate.canPlace(part)) {
                            if (!found || score < betterScore) {
                                found = true;
                                betterX = x;
                                betterY = y;
                                betterScore = score;
                                betterR = r;
                            }
                        }
                    }
                }
            }
            if (found) {
                _log("- Placing it @%d,%d rotation=%d\n", betterX, betterY, betterR);
                part->setRotation(betterR);
                part->setOffset(betterX, betterY);
                plate.place(part);
            } else {
                _log("! Can't place it (TODO: create a new plate)\n");
            }
        }

        plate.bmp->toPpm();
    }
}
