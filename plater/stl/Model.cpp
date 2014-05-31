#include "Model.h"

namespace Plater
{

    Model::Model()
        : tree(NULL)
    {
    }

    Model::~Model()
    {
        if (tree != NULL) {
            delete tree;
        }

        for (unsigned int i=0; i<triangles.size(); i++) {
            delete triangles[i];
        }
    }

    Point3 Model::min(){
    	if (volumes.size() < 1)
            return Point3(0, 0, 0);

        Point3 min = volumes[0].min();

        int xmin = min.x;
        int ymin = min.y;
        int zmin = min.z;

        for (auto v : volumes) {
            Point3 p = v.min();
            if(p.x < xmin)
            	xmin = p.x;
            if(p.y < ymin)
            	ymin = p.y;
            if(p.z < zmin)
            	zmin = p.z;
        }
        return Point3(xmin, ymin, zmin);
    }

    Point3 Model::max(){
    	if (volumes.size() < 1)
            return Point3(0, 0, 0);

        Point3 max = volumes[0].max();

        int xmax = max.x;
        int ymax = max.y;
        int zmax = max.z;

        for (auto v : volumes) {
            Point3 p = v.max();
            if(p.x > xmax)
            	xmax = p.x;
            if(p.y > ymax)
            	ymax = p.y;
            if(p.z > zmax)
            	zmax = p.z;
        }
        return Point3(xmax, ymax, zmax);
    }

    bool Model::contains(float x, float y)
    {
        if (tree == NULL) {
            Point3 minP = min();
            Point3 maxP = max();
            tree = new Plater::QuadTree(minP.x, minP.y, maxP.x, maxP.y, 6);

            for(unsigned int i=0; i<volumes.size(); i++)
            {
                for (unsigned int k=0; k<volumes[i].faces.size(); k++) {
                    Face &face = volumes[i].faces[k];
                    Triangle *triangle = new Triangle(
                            FPoint2(face.v[0].x, face.v[0].y),
                            FPoint2(face.v[1].x, face.v[1].y),
                            FPoint2(face.v[2].x, face.v[2].y)
                            );
                    triangles.push_back(triangle);
                    tree->add(triangle);
                }
            }
        } 

        return tree->test(x, y);
    }
    Bitmap *Model::pixelize(float precision, float dilatation)
    {
        Point3 minP = min();
        Point3 maxP = max();
        float xMin = minP.x-dilatation;
        float yMin = minP.y-dilatation;
        float xMax = maxP.x+dilatation;
        float yMax = maxP.y+dilatation;
        int width = (xMax-xMin)/precision;
        int height = (yMax-yMin)/precision;
        Bitmap *bitmap = new Bitmap(width, height);

        for (int x=0; x<width; x++) {
            for (int y=0; y<height; y++) {
                float X = (x+1)*precision - dilatation + minP.x;
                float Y = (y+1)*precision - dilatation + minP.y;
                if (X > minP.x && X < maxP.x && Y > minP.y && Y < maxP.y) {
                    bitmap->setPoint(x, y, contains(X, Y) ? 2 : 0);
                } else {
                    bitmap->setPoint(x, y, 0);
                }
            }
        }

        bitmap->dilatation(dilatation/precision);

        return bitmap;
    }
        
    Model Model::translate(float X, float Y, float Z)
    {
        Model translated;
        translated.volumes = volumes;

        for (auto& volume : translated.volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    face.v[i].x += X;
                    face.v[i].y += Y;
                    face.v[i].z += Z;
                }
            }
        }

        return translated;
    }
        
    void Model::merge(const Model &other)
    {
        for (auto volume : other.volumes) {
            volumes.push_back(volume);
        }
    }
        
    Model Model::rotateZ(float r)
    {
        Model rotated;
        rotated.volumes = volumes;

        for (auto& volume : rotated.volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    float x = face.v[i].x;
                    float y = face.v[i].y;
                    face.v[i].x = cos(r)*x-sin(r)*y;
                    face.v[i].y = sin(r)*x+cos(r)*y;
                }
            }
        }

        return rotated;
    }
        
    Model Model::rotateY(float r)
    {
        Model rotated;
        rotated.volumes = volumes;

        for (auto& volume : rotated.volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    float x = face.v[i].x;
                    float z = face.v[i].z;
                    face.v[i].x = cos(r)*x-sin(r)*z;
                    face.v[i].z = sin(r)*x+cos(r)*z;
                }
            }
        }

        return rotated;
    }
        
    Model Model::rotateX(float r)
    {
        Model rotated;
        rotated.volumes = volumes;

        for (auto& volume : rotated.volumes) {
            for (auto& face : volume.faces) {
                for (int i=0; i<3; i++) {
                    float y = face.v[i].y;
                    float z = face.v[i].z;
                    face.v[i].y = cos(r)*y-sin(r)*z;
                    face.v[i].z = sin(r)*y+cos(r)*z;
                }
            }
        }

        return rotated;
    }

    Model Model::center()
    {
        Point3 minP = min();
        Point3 maxP = max();

        float X = (minP.x+maxP.x)/2.0;
        float Y = (minP.y+maxP.y)/2.0;
        float Z = minP.z;

        return translate(-X, -Y, -Z);
    }
        
    Model Model::putFaceOnPlate(string orientation)
    {
        if (orientation == "front") return rotateX(DEG2RAD(90));
        if (orientation == "top") return rotateX(DEG2RAD(180));
        if (orientation == "back") return rotateX(DEG2RAD(270));
        if (orientation == "left") return rotateY(DEG2RAD(90));
        if (orientation == "right") return rotateY(DEG2RAD(-90));
        return *this;
    }

}