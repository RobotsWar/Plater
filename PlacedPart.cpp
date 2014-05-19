#include "PlacedPart.h"

namespace Plater
{
    PlacedPart::PlacedPart()
        : part(NULL), x(0), y(0), dirty(true), bmp(NULL), rotation(1.4)
    {
    }

    PlacedPart::~PlacedPart()
    {
        if (bmp != NULL) {
            delete bmp;
        }
    }

    void PlacedPart::setPart(Part *part_)
    {
        dirty = true;
        part = part_;
    }

    void PlacedPart::setOffset(float x_, float y_)
    {
        x = x_;
        y = y_;
    }

    void PlacedPart::setRotation(float r)
    {
        dirty = true;
        rotation = r;
    }

    Part *PlacedPart::getPart()
    {
        return part;
    }

    float PlacedPart::getX()
    {
        return x;
    }

    float PlacedPart::getY()
    {
        return y;
    }

    Bitmap *PlacedPart::generateBitmap()
    {
        Bitmap *rotated = Bitmap::rotate(part->getBmp(), rotation);
        Bitmap *trimmed = Bitmap::trim(rotated);
        delete rotated;
        return trimmed;
    }
    
    Bitmap *PlacedPart::getBmp()
    {
        if (dirty) {
            dirty = false;
            if (bmp != NULL) {
                delete bmp;
            }
            bmp = generateBitmap();
        }

        return bmp;
    }
            
    float PlacedPart::getGX()
    {
        getBmp();
        return (bmp->sX/(float)bmp->pixels)*part->precision;
    }

    float PlacedPart::getGY()
    {
        getBmp();
        return (bmp->sY/(float)bmp->pixels)*part->precision;
    }
}
