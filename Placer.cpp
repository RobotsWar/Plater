#include <algorithm>
#include <random>
#include <chrono>
#include "Placer.h"
#include "log.h"

namespace Plater
{
    Placer::Placer(Request *request_)
        : request(request_)
    {
        for (auto part : request->quantities) {
            for (int i=0; i<part.second; i++) {
                PlacedPart *placedPart = new PlacedPart;
                placedPart->setPart(request->parts[part.first]);
                parts.push_back(placedPart);
            }
        }
    }
            
    void Placer::sortParts(int sortType)
    {
        switch (sortType) {
            case PLACER_SORT_SURFACE_INC:
                sort(parts.begin(), parts.end(), [](const PlacedPart *a, const PlacedPart *b) {
                    return a->getSurface() > b->getSurface();
                });
                break;
            case PLACER_SORT_SURFACE_DEC:
                sort(parts.begin(), parts.end(), [](const PlacedPart *a, const PlacedPart *b) {
                    return a->getSurface() < b->getSurface();
                });
                break;
            default:
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                shuffle(parts.begin(), parts.end(), std::default_random_engine(seed));
                break;
        }
    }

    PlacedPart *Placer::getNextPart()
    {
        PlacedPart *part = parts.back();
        parts.pop_back();

        return part;
    }

    Plate *Placer::place()
    {
        Plate *plate = new Plate(request->plateWidth, request->plateHeight, request->precision);

        _log("* Placer\n");
        while (parts.size()) {
            PlacedPart *part = getNextPart();
            _log("- Trying to place %s...\n", part->getPart()->getFilename().c_str());
            float betterX=0, betterY=0, betterScore;
            int betterR=0;
            int rs = (M_PI*2/request->deltaR);
            bool found = false;
            for (int r=0; r<rs; r++) {
                part->setRotation(r);
                for (float x=0; x<plate->width; x+=request->delta) {
                    for (float y=0; y<plate->height; y+=request->delta) {
                        float gx = part->getGX()+x;
                        float gy = part->getGY()+y;
                        part->setOffset(x, y);

                        float score = gy*10+gx;

                        if (plate->canPlace(part)) {
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
                _log("- Placing it @%g,%g r=%d\n", betterX, betterY, betterR);
                part->setRotation(betterR);
                part->setOffset(betterX, betterY);
                plate->place(part);
            } else {
                _log("! Can't place it (TODO: create a new plate)\n");
            }

        }
        
        return plate;
    }
}
