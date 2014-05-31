#ifndef VOLUME_H
#define VOLUME_H

#include "Face.h"
#include <vector>
using std::vector;

namespace Plater
{
    class Volume
    {
        public:
			vector<Face> faces;
			Volume();
			void addFace(Face f);
			Point3 min();
			Point3 max();
    };
}
#endif