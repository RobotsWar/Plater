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

    void Part::load(std::string filename_)
    {
        filename = filename_;
        FMatrix3x3 id;
        model = loadModelFromFile(filename.c_str(), id);
        bmp = model->pixelize(500, 1000);
    }
            
    bool Part::overlaps(const Part &other)
    {
    }
}
