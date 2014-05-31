#ifndef FACE_H
#define FACE_H

#include "Point3.h"

namespace Plater
{
    class Face
    {
        public:
			Point3 v[3];
			Face();
			Face(Point3 v0, Point3 v1, Point3 v2);
    };
}
#endif