#ifndef _PLATER_PART_H
#define _PLATER_PART_H

#include <iostream>
#include "modelFile/modelFile.h"

namespace Plater
{
    class Part
    {
        public:
            Part();
            virtual ~Part();

            void load(std::string filename, float precision);
            std::string getFilename();

            SimpleModel *model;

            Bitmap *getBmp();
            float getSurface();

            float width, height;

            float precision;

        protected:
            Bitmap *bmp;
            std::string filename;
    };
}

#endif
