#ifndef _PLATER_QUADTREE_H
#define _PLATER_QUADTREE_H

#include <vector>
#include "Rectangle.h"
#include "Triangle.h"

namespace Plater
{
    class QuadTree
    {
        public:
            QuadTree(float x1, float y1, float x2, float y2, int depth);
            virtual ~QuadTree();

            void add(Triangle *t);
            bool test(float x, float y);
            void get(float x, float y, std::vector<Triangle *> &all);
            
            std::vector<Triangle *> triangles;

        protected:
            int depth;
            Rectangle r;
            QuadTree *quad1, *quad2, *quad3, *quad4;
            bool black;
    };
}

#endif
