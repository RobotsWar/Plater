#include "Volume.h"
#include <iostream>
using namespace std;
namespace Plater
{
    Volume::Volume()
    {
    }

    void Volume::addFace(Face f){
        faces.push_back(f);
    }

    Point3 Volume::min(){
        if (faces.size() < 1)
            return Point3(0, 0, 0);

        int xmin = faces[0].v[0].x;
        int ymin = faces[0].v[0].y;
        int zmin = faces[0].v[0].z;

        for (auto f : faces) {
            for (int i=0; i<3; i++) {
                if(f.v[i].x < xmin)
                    xmin = f.v[i].x;
                if(f.v[i].y < ymin)
                    ymin = f.v[i].y;
                if(f.v[i].z < zmin)
                    zmin = f.v[i].z;
            }
        }
        return Point3(xmin, ymin, zmin);
    }

    Point3 Volume::max(){
        if (faces.size() < 1)
            return Point3(0, 0, 0);

        int xmax = faces[0].v[0].x;
        int ymax = faces[0].v[0].y;
        int zmax = faces[0].v[0].z;

        for (auto f : faces) {
            for (int i=0; i<3; i++) {
                if(f.v[i].x > xmax)
                    xmax = f.v[i].x;
                if(f.v[i].y > ymax)
                    ymax = f.v[i].y;
                if(f.v[i].z > zmax)
                    zmax = f.v[i].z;
            }
        }
        return Point3(xmax, ymax, zmax);
        
    }
}