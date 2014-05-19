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
            delete bmp;
        }
    }

    void Part::load(std::string filename_, float precision)
    {
        filename = filename_;
        FMatrix3x3 id;
        model = loadModelFromFile(filename.c_str(), id);
        bmp = model->pixelize(precision, 1000);        

        width = bmp->width*precision;
        height = bmp->height*precision;
    }

    std::string Part::getFilename()
    {
        return filename;
    }
            
    Bitmap *Part::getBmp()
    {
        return bmp;
    }
}
