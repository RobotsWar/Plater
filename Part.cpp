#include <iostream>
#include "Part.h"
#include "modelFile/modelFile.h"

using namespace std;

namespace Plater
{
    Part::Part()
        : model(NULL)
    {
    }

    Part::~Part()
    {
        if (model != NULL) {
            delete model;
        }
        if (bmp != NULL) {
            delete[] bmp;
        }
    }

    void Part::load(std::string filename_, float precision_, float deltaR)
    {
        int bmps = (M_PI*2)/deltaR;
        precision = precision_;
        filename = filename_;
        FMatrix3x3 id;
        model = loadModelFromFile(filename.c_str(), id);
        bmp = new Bitmap*[bmps];
        bmp[0] = model->pixelize(precision, 2500);

        for (int k=1; k<bmps; k++) {
            Bitmap *rotated = Bitmap::rotate(bmp[0], k*deltaR);
            bmp[k] = Bitmap::trim(rotated);
        }
    }
            
    float Part::getSurface() const
    {
        return bmp[0]->width*bmp[0]->height;
    }

    std::string Part::getFilename()
    {
        return filename;
    }
            
    Bitmap *Part::getBmp(int index) const
    {
        return bmp[index];
    }
            
    float Part::getDensity(int index) const
    {
        Bitmap *bmp = getBmp(index);
        return bmp->pixels/(bmp->width*bmp->height);
    }
}
