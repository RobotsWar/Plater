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

            void load(std::string filename, float precision, float deltaR, float spacing);
            std::string getFilename();

            SimpleModel *model;

            Bitmap *getBmp(int index) const;
            float getSurface() const;
            float getDensity(int index) const;

            float precision;
            float deltaR;

        protected:
            int bmps;
            Bitmap **bmp;
            std::string filename;
    };
}

#endif
