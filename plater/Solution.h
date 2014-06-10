#ifndef _PLATER_SOLUTION_H
#define _PLATER_SOLUTION_H

#include <vector>
#include "Plate.h"

namespace Plater
{
    class Solution
    {
        public:
            Solution(float plateWidth_, float plateHeight_, float plateDiameter_, int plateMode_, float precision_);
            virtual ~Solution();
            float score();

            int countPlates();
            Plate *getPlate(int index);
            Plate *lastPlate();
            void addPlate();

        protected:
            float plateWidth, plateHeight, plateDiameter;
            int plateMode;
            float precision;
            std::vector<Plate*> plates;
    };
}

#endif
