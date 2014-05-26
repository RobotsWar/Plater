#include <stdlib.h>
#include "QuadTree.h"

namespace Plater
{
    QuadTree::QuadTree(float x1, float y1, float x2, float y2, int depth_)
        :
        depth(depth_),
        quad1(NULL), quad2(NULL), quad3(NULL), quad4(NULL),
        black(false)
    {
        r.x1 = x1;
        r.y1 = y1;
        r.x2 = x2;
        r.y2 = y2;

        float xm = (x1+x2)/2.0;
        float ym = (y1+y2)/2.0;

        if (depth > 0) {
            quad1 = new QuadTree(x1, y1, xm, ym, depth-1);
            quad2 = new QuadTree(xm, y1, x2, ym, depth-1);
            quad3 = new QuadTree(x1, ym, xm, y2, depth-1);
            quad4 = new QuadTree(xm, ym, x2, y2, depth-1);
        }
    }
            
    void QuadTree::add(Triangle *t)
    {
        if (depth > 0) {
            if (black) {
                return;
            }
            if (t->contains(r)) {
                black = true;
                delete quad1;
                delete quad2;
                delete quad3;
                delete quad4;
                return;
            }
            if (t->box.overlaps(r)) {
                quad1->add(t);
                quad2->add(t);
                quad3->add(t);
                quad4->add(t);
            }
        } else {
            triangles.push_back(t);
        }
    }
            
    void QuadTree::get(float x, float y, std::vector<Triangle *> &all)
    {
        if (r.contains(x, y)) {
            if (depth > 0) {
                    quad1->get(x, y, all);
                    quad2->get(x, y, all);
                    quad3->get(x, y, all);
                    quad4->get(x, y, all);
            } else {
                for (unsigned int i=0; i<triangles.size(); i++) {
                    all.push_back(triangles[i]);
                }
            }
        }
    }
    
    bool QuadTree::test(float x, float y)
    {
        if (r.contains(x, y)) {
            if (black) {
                return true;
            }
            if (depth > 0) {
                return quad1->test(x,y)
                    || quad2->test(x,y)
                    || quad3->test(x,y)
                    || quad4->test(x,y);
            } else {
                for (unsigned int i=0; i<triangles.size(); i++) {
                    if (triangles[i]->contains(x, y)) {
                        return true;
                    }
                }

                return false;
            }
        } else {
            return false;
        }
    }
            
    QuadTree::~QuadTree()
    {
        if (depth > 0 && !black) {
            delete quad1;
            delete quad2;
            delete quad3;
            delete quad4;
        }
    }
}
