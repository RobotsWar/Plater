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

            void load(std::string filename);
            std::string getFilename();

            bool overlaps(const Part &other);
            SimpleModel *model;

        protected:
            Bitmap *bmp;
            std::string filename;
    };
}

#endif
