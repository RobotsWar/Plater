#include <sstream>
#include <iostream>
#include <unistd.h>
#include "Bitmap.h"

using namespace std;

namespace Plater
{
    Bitmap::Bitmap(int width_, int height_)
        : data(NULL), width(width_), height(height_), sX(0), sY(0), pixels(0)
    {
        data = new bool[width*height];

        for (int x=0; x<width; x++) {
            for (int y=0; y<height; y++) {
                data[BMP_POSITION(x,y)] = false;
            }
        }
    }
            
    Bitmap::Bitmap(const Bitmap *other)
    {
        width = other->width;
        height = other->height;
        data = new bool[width*height];

        for (int x=0; x<width; x++) {
            for (int y=0; y<height; y++) {
                data[BMP_POSITION(x,y)] = other->data[BMP_POSITION(x,y)];
            }
        }
    }

    Bitmap::~Bitmap()
    {
        if (data != NULL) {
            delete[] data;
        }
    }
            
    string Bitmap::toPpm()
    {
        ostringstream oss;
        oss << "P1" << endl;
        oss << width << " " << height << endl;
        for (int y=0; y<height; y++) {
            for (int x=0; x<width; x++) {
                oss << data[BMP_POSITION(x,y)] << " ";
            }
            if (y+1 != height) {
                oss << "\n";
            }
        }
        return oss.str();
    }
}
