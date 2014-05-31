#include "PlacedPart.h"

namespace Plater
{
    PlacedPart::PlacedPart()
        : part(NULL), x(0), y(0), rotation(0)
    {
    }

    PlacedPart::~PlacedPart()
    {
    }
            
    Model PlacedPart::createModel()
    {
        Model model = part->model.center().rotateZ(part->deltaR*rotation);

        return model.translate(getCenterX(), getCenterY());
    }
    
    float PlacedPart::getCenterX()
    {
        return x+part->precision*getBmp()->centerX;
    }

    float PlacedPart::getCenterY()
    {
        return y+part->precision*getBmp()->centerY;
    }

    std::string PlacedPart::getName()
    {
        return part->getFilename();
    }
            
    float PlacedPart::getSurface() const
    {
        return part->getSurface();
    }
            
    float PlacedPart::getGDist() const
    {
        bool hasScore = false;
        float score = 0;
        for (int i=0; i<part->bmps; i++) {
            Bitmap *bmp = part->bmp[i];
            float gX = bmp->sX/(float)bmp->pixels;
            float gY = bmp->sY/(float)bmp->pixels;
            float s = gX*gX+gY*gY;
            if (!hasScore || s<score) {
                score = s;
                hasScore = true;
            }
        }

        return score;
    }

    void PlacedPart::setPart(Part *part_)
    {
        part = part_;
    }

    void PlacedPart::setOffset(float x_, float y_)
    {
        x = x_;
        y = y_;
    }

    void PlacedPart::setRotation(int r)
    {
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

    int PlacedPart::getRotation()
    {
        return rotation;
    }
    
    Bitmap *PlacedPart::getBmp() const
    {
        return part->getBmp(rotation);
    }
            
    float PlacedPart::getGX() const
    {
        Bitmap *bmp = getBmp();
        return (bmp->sX/(float)bmp->pixels)*part->precision;
    }

    float PlacedPart::getGY() const
    {
        Bitmap *bmp = getBmp();
        return (bmp->sY/(float)bmp->pixels)*part->precision;
    }
}
