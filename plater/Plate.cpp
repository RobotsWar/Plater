#include <iostream>
#include "log.h"
#include "Plate.h"

using namespace std;

namespace Plater
{
    Plate::Plate(float width_, float height_, float precision_)
        : width(width_), height(height_), precision(precision_)
    {
        bmp = new Bitmap(width/precision, height/precision);
    }
            
    Model Plate::createModel()
    {
        Model model;

        for (auto part : parts) {
            model.merge(part->createModel());
        }

        return model.center();
    }

    Plate::~Plate()
    {
        if (bmp != NULL) {
            delete bmp;
        }

        for (auto placed : parts) {
            delete placed;
        }
    }

    bool Plate::canPlace(PlacedPart *placedPart)
    {
        Bitmap *partBmp = placedPart->getBmp();
        float x = placedPart->getX();
        float y = placedPart->getY();

        if ((x+partBmp->width*precision) > width || (y+partBmp->height*precision) > height) {
            return false;
        }

        return !partBmp->overlaps(
                bmp, 
                x/precision, 
                y/precision
                );
    }
            
    void Plate::place(PlacedPart *placedPart)
    {
        parts.push_back(placedPart);
        bmp->write(placedPart->getBmp(), placedPart->getX()/precision, placedPart->getY()/precision);
    }
            
    int Plate::countParts()
    {
        return parts.size();
    }
}
