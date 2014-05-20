/** Copyright (C) 2013 David Braam - Released under terms of the AGPLv3 License */
#include <fstream>
#include <string.h>
#include <stdio.h>

#include "modelFile.h"
#include "../log.h"

using namespace std;
using namespace Plater;

FILE* binaryMeshBlob = NULL;

/* Custom fgets function to support Mac line-ends in Ascii STL files. OpenSCAD produces this when used on Mac */
void* fgets_(char* ptr, size_t len, FILE* f)
{
    while(len && fread(ptr, 1, 1, f) > 0)
    {
        if (*ptr == '\n' || *ptr == '\r')
        {
            *ptr = '\0';
            return ptr;
        }
        ptr++;
        len--;
    }
    return NULL;
}

void saveModelToFileAscii(const char *filename, SimpleModel *model)
{
    ofstream ofile(filename);

    ofile << "solid plate" << endl;
    for (auto volume : model->volumes) {
        for (auto face : volume.faces) {
            ofile << "  facet normal 1 0 0" << endl;
            ofile << "    outer loop" << endl;
            for (int i=0; i<3; i++) {
                ofile << "      vertex " << 
                    face.v[i].x/1000.0 << " " <<
                    face.v[i].y/1000.0 << " " <<
                    face.v[i].z/1000.0 << endl;
            }
            ofile << "    endloop" << endl;
            ofile << "  endfacet" << endl;
        }
    }
    ofile << "endsolid plate" << endl;

    ofile.close();
}

SimpleModel* loadModelSTL_ascii(const char* filename, FMatrix3x3& matrix)
{
    SimpleModel* m = new SimpleModel();
    m->volumes.push_back(SimpleVolume());
    SimpleVolume* vol = &m->volumes[0];
    FILE* f = fopen(filename, "rt");
    char buffer[1024];
    FPoint3 vertex;
    int n = 0;
    Point3 v0(0,0,0), v1(0,0,0), v2(0,0,0);
    while(fgets_(buffer, sizeof(buffer), f))
    {
        if (sscanf(buffer, " vertex %lf %lf %lf", &vertex.x, &vertex.y, &vertex.z) == 3)
        {
            n++;
            switch(n)
            {
                case 1:
                    v0 = matrix.apply(vertex);
                    break;
                case 2:
                    v1 = matrix.apply(vertex);
                    break;
                case 3:
                    v2 = matrix.apply(vertex);
                    vol->addFace(v0, v1, v2);
                    n = 0;
                    break;
            }
        }
    }
    fclose(f);
    return m;
}

void saveModelToFileBinary(const char *filename, SimpleModel *model)
{
    ofstream ofile(filename,ios::binary);
    char h = '\0';
    for (int i = 0; i < 80; i++)
    {
        ofile.write(&h, sizeof(char));
    }
    uint32_t faceCount = 0;
    for (auto volume : model->volumes) {
        faceCount += volume.faces.size();
    }
    ofile.write((char*)&faceCount, sizeof(uint32_t));
    for (auto volume : model->volumes) {
        for (auto face : volume.faces) {
            float a = 1.0;
            float b = 0.0;
            ofile.write((char*)&a, sizeof(float));
            ofile.write((char*)&b, sizeof(float));
            ofile.write((char*)&b, sizeof(float));
            for (int i=0; i<3; i++) {
                float x = face.v[i].x/1000.0;
                float y = face.v[i].y/1000.0;
                float z = face.v[i].z/1000.0;
                ofile.write((char*)&x, sizeof(float));
                ofile.write((char*)&y, sizeof(float));
                ofile.write((char*)&z, sizeof(float));
            }
            uint16_t flags = 0;
            ofile.write((char*)&flags, sizeof(uint16_t));
        }
    }

    ofile.close();
}

SimpleModel* loadModelSTL_binary(const char* filename, FMatrix3x3& matrix)
{
    FILE* f = fopen(filename, "rb");
    char buffer[80];
    uint32_t faceCount;
    //Skip the header
    if (fread(buffer, 80, 1, f) != 1)
    {
        fclose(f);
        return NULL;
    }
    //Read the face count
    if (fread(&faceCount, sizeof(uint32_t), 1, f) != 1)
    {
        fclose(f);
        return NULL;
    }
    //For each face read:
    //float(x,y,z) = normal, float(X,Y,Z)*3 = vertexes, uint16_t = flags
    SimpleModel* m = new SimpleModel();
    m->volumes.push_back(SimpleVolume());
    SimpleVolume* vol = &m->volumes[0];
    if(vol == NULL)
    {
        fclose(f);
        return NULL;
    }
    for(unsigned int i=0;i<faceCount;i++)
    {
        if (fread(buffer, sizeof(float) * 3, 1, f) != 1)
        {
            fclose(f);
            return NULL;
        }
        float v[9];
        if (fread(v, sizeof(float) * 9, 1, f) != 1)
        {
            fclose(f);
            return NULL;
        }
        Point3 v0 = matrix.apply(FPoint3(v[0], v[1], v[2]));
        Point3 v1 = matrix.apply(FPoint3(v[3], v[4], v[5]));
        Point3 v2 = matrix.apply(FPoint3(v[6], v[7], v[8]));
        vol->addFace(v0, v1, v2);
        if (fread(buffer, sizeof(uint16_t), 1, f) != 1)
        {
            fclose(f);
            return NULL;
        }
    } 
    fclose(f);
    return m;
}

SimpleModel* loadModelSTL(const char* filename, FMatrix3x3& matrix)
{
    FILE* f = fopen(filename, "r");
    char buffer[6];
    if (f == NULL)
        return NULL;

    if (fread(buffer, 5, 1, f) != 1)
    {
        fclose(f);
        return NULL;
    }
    fclose(f);

    buffer[5] = '\0';
    if (strcasecmp(buffer, "SOLID") == 0)
    {
        return loadModelSTL_ascii(filename, matrix);
    }
    return loadModelSTL_binary(filename, matrix);
}

SimpleModel* loadModelFromFile(const char* filename, FMatrix3x3& matrix)
{
    const char* ext = strrchr(filename, '.');
    if (ext && strcasecmp(ext, ".stl") == 0)
    {
        return loadModelSTL(filename, matrix);
    }
    if (filename[0] == '#' && binaryMeshBlob != NULL)
    {
        SimpleModel* m = new SimpleModel();

        while(*filename == '#')
        {
            filename++;

            m->volumes.push_back(SimpleVolume());
            SimpleVolume* vol = &m->volumes[m->volumes.size()-1];
            int32_t n, pNr = 0;
            if (fread(&n, 1, sizeof(int32_t), binaryMeshBlob) < 1)
                return NULL;
            log("Reading mesh from binary blob with %i vertexes\n", n);
            Point3 v[3];
            while(n)
            {
                float f[3];
                if (fread(f, 3, sizeof(float), binaryMeshBlob) < 1)
                    return NULL;
                FPoint3 fp(f[0], f[1], f[2]);
                v[pNr++] = matrix.apply(fp);
                if (pNr == 3)
                {
                    vol->addFace(v[0], v[1], v[2]);
                    pNr = 0;
                }
                n--;
            }
        }
        return m;
    }
    return NULL;
}

bool SimpleModel::contains(float x, float y)
{
    for(unsigned int i=0; i<volumes.size(); i++)
    {
        for (unsigned int k=0; k<volumes[i].faces.size(); k++) {
            SimpleFace &face = volumes[i].faces[k];
            Triangle t(
                    FPoint2(face.v[0].x, face.v[0].y),
                    FPoint2(face.v[1].x, face.v[1].y),
                    FPoint2(face.v[2].x, face.v[2].y)
                    );

            if (t.contains(x, y)) {
                return true;
            }
        }
    }

    return false;
}

Bitmap *SimpleModel::pixelize(float precision, float dilatation)
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
    
SimpleModel SimpleModel::translate(float X, float Y, float Z)
{
    SimpleModel translated;
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
    
void SimpleModel::merge(const SimpleModel &other)
{
    for (auto volume : other.volumes) {
        volumes.push_back(volume);
    }
}
    
SimpleModel SimpleModel::rotate(float r)
{
    SimpleModel rotated;
    rotated.volumes = volumes;

    r = -r;
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

SimpleModel SimpleModel::center()
{
    Point3 minP = min();
    Point3 maxP = max();

    float X = (minP.x+maxP.x)/2.0;
    float Y = (minP.y+maxP.y)/2.0;
    float Z = minP.z;

    return translate(-X, -Y, -Z);
}
