#ifndef MODEL_H
#define MODEL_H

#include "Volume.h"
#include "Point3.h"
#include "../Bitmap.h"
#include "../Triangle.h"
#include "../QuadTree.h"
#include <vector>
using std::vector;

namespace Plater
{
    class Model
    {
        public:
			vector<Volume> volumes;
			Model();
			virtual ~Model();
			Point3 min();
			Point3 max();
			bool contains(float x, float y);
			Bitmap *pixelize(float precision, float dilatation);
    		Model translate(float X=0, float Y=0, float Z=0);
			void merge(const Model &other);
			Model rotateZ(float r);
			Model rotateY(float r);
			Model rotateX(float r);
			Model center();
			Model putFaceOnPlate(string orientation);

			Plater::QuadTree *tree;
    		vector<Plater::Triangle*> triangles;
    };
}
#endif