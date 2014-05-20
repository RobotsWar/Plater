#include "Solution.h"

namespace Plater
{
    Solution::Solution(float plateWidth_, float plateHeight_, float precision_)
        : plateWidth(plateWidth_), plateHeight(plateHeight_), precision(precision_)
    {
    }

    Solution::~Solution()
    {
        for (auto plate : plates) {
            delete plate;
        }
    }

    Plate *Solution::getPlate(int index)
    {
        if (((unsigned int)index) < plates.size()) {
            return plates[index];
        } else {
            return NULL;
        }
    }

    void Solution::addPlate()
    {
        plates.push_back(new Plate(plateWidth, plateHeight, precision));
    }

    int Solution::countPlates()
    {
        return plates.size();
    }

    float Solution::score()
    {
        return countPlates();
    }
}
