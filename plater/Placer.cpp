#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <random>
#include <chrono>
#include "Placer.h"
#include "log.h"

namespace Plater
{
    Placer::Placer(Request *request_)
        : rotateDirection(0),
        request(request_)
    {
        for (auto part : request->quantities) {
            for (int i=0; i<part.second; i++) {
                PlacedPart *placedPart = new PlacedPart;
                placedPart->setPart(request->parts[part.first]);
                parts.push_back(placedPart);
            }
        }

        setGravityMode(PLACER_GRAVITY_YX);
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
            case PLACER_SORT_GDIST_INC:
                sort(parts.begin(), parts.end(), [](const PlacedPart *a, const PlacedPart *b) {
                        return a->getGDist() > b->getGDist();
                        });
                break;
            case PLACER_SORT_GDIST_DEC:
                sort(parts.begin(), parts.end(), [](const PlacedPart *a, const PlacedPart *b) {
                        return a->getGDist() < b->getGDist();
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
            
    void Placer::setRotateDirection(int direction)
    {
        rotateDirection = direction;
    }
            
    void Placer::setRotateOffset(int offset)
    {
        rotateOffset = offset;
    }
            
    void Placer::setGravityMode(int gravityMode)
    {
        switch (gravityMode) {
            case PLACER_GRAVITY_YX:
                xCoef = 1;
                yCoef = 10;
                break;
            case PLACER_GRAVITY_XY:
                xCoef = 1;
                yCoef = 10;
                break;
            case PLACER_GRAVITY_EQ:
                xCoef = 1;
                yCoef = 1;
                break;
        }
    }
    
    bool Placer::placePart(Plate *plate, PlacedPart *part)
    {
        std::string cacheName = part->getName();

        if (!cache[plate][cacheName]) {
            float betterX=0, betterY=0, betterScore;
            int betterR=0;
            int rs = ceil(M_PI*2/request->deltaR);
            bool found = false;

            for (int r=(rotateDirection ? rs-1 : 0); rotateDirection ? r>=0 : r<rs; rotateDirection ? r-- : r++) {
                int vr = (r+rotateOff)%rs;
                part->setRotation(vr);
                for (float x=0; x<plate->width; x+=request->delta) {
                    for (float y=0; y<plate->height; y+=request->delta) {
                        float gx = part->getGX()+x;
                        float gy = part->getGY()+y;
                        float score = gy*yCoef+gx*xCoef;

                        if (!found || score < betterScore) {
                            part->setOffset(x, y);
                            if (plate->canPlace(part)) {
                                found = true;
                                betterX = x;
                                betterY = y;
                                betterScore = score;
                                betterR = vr;
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
                return true;
            } else {
                cache[plate][cacheName] = true;
                return false;
            }
        } else {
            return false;
        }
    }

    Solution *Placer::place()
    {
        Solution *solution = new Solution(request->plateWidth, request->plateHeight, request->precision);
        solution->addPlate();

        _log("* Placer\n");
        while (parts.size()) {
            PlacedPart *part = getNextPart();
            _log("- Trying to place %s...\n", part->getPart()->getFilename().c_str());
            bool placed = false;

            for (int i=0; i<solution->countPlates() && !placed; i++) {
                Plate *plate = solution->getPlate(i);

                if (placePart(plate, part)) {
                    placed = true;
                } else {
                    if (i+1 == solution->countPlates()) {
                        _log("! Creating a new plate\n");
                        solution->addPlate();
                    }
                }
            }
        }

        return solution;
    }
}
