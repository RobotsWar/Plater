#include <cstdlib>
#include <cstdio>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
  #define snprintf _snprintf
  #define vsnprintf _vsnprintf
  #define strcasecmp _stricmp
  #define strncasecmp _strnicmp
#endif

#include "StlFactory.h"

using namespace std;
using namespace Plater;


FILE* binaryMeshBlob = NULL;

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

void saveModelToFileAscii(const char *filename, Model *model)
{
    ofstream ofile(filename);
    
    if (!ofile) {
        ostringstream oss;
        oss << "Can't open file " << filename << " for writing";
        throw oss.str();
    }

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

Model loadModelSTL_ascii(const char* filename)
{
    Model m;
    m.volumes.push_back(Volume());
    Volume* vol = &m.volumes[0];
    FILE* f = fopen(filename, "rt");

    if (f == NULL) {
        ostringstream oss;
        oss << "Can't open file " << filename << " for reading";
        throw oss.str();
    }

    std::locale::global(std::locale("C"));
    char buffer[1024];
    Point3 vertex;
    int n = 0;
    Point3 v0(0,0,0), v1(0,0,0), v2(0,0,0);
    while(fgets_(buffer, sizeof(buffer), f))
    {
        if (sscanf(buffer, " vertex %lf %lf %lf", &vertex.x, &vertex.y, &vertex.z) == 3)
        {
            vertex.x*=1000;
            vertex.y*=1000;
            vertex.z*=1000;
            n++;
            switch(n)
            {
                case 1:
                    v0 = vertex;
                    break;
                case 2:
                    v1 = vertex;
                    break;
                case 3:
                    v2 = vertex;
                    vol->addFace(Face(v0, v1, v2));
                    n = 0;
                    break;
            }
        }
    }
    fclose(f);
    return m;
}

void saveModelToFileBinary(const char *filename, Model *model)
{
    ofstream ofile(filename,ios::binary);

    if (!ofile) {
        ostringstream oss;
        oss << "Can't open file " << filename << " for writing";
        throw oss.str();
    }

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

Model loadModelSTL_binary(const char* filename)
{
    Model m;
    FILE* f = fopen(filename, "rb");

    if (f == NULL) {
        ostringstream oss;
        oss << "Can't open file " << filename << " for reading";
        throw oss.str();
    }

    char buffer[80];
    uint32_t faceCount;
    //Skip the header
    if (fread(buffer, 80, 1, f) != 1)
    {
        fclose(f);
        return m;
    }
    //Read the face count
    if (fread(&faceCount, sizeof(uint32_t), 1, f) != 1)
    {
        fclose(f);
        return m;
    }
    //For each face read:
    //float(x,y,z) = normal, float(X,Y,Z)*3 = vertexes, uint16_t = flags
    m.volumes.push_back(Volume());
    Volume* vol = &m.volumes[0];
    if(vol == NULL)
    {
        fclose(f);
        return m;
    }
    for(unsigned int i=0;i<faceCount;i++)
    {
        if (fread(buffer, sizeof(float) * 3, 1, f) != 1)
        {
            fclose(f);
            return m;
        }
        float v[9];
        if (fread(v, sizeof(float) * 9, 1, f) != 1)
        {
            fclose(f);
            return m;
        }
        Point3 v0 = Point3(v[0]*1000, v[1]*1000, v[2]*1000);
        Point3 v1 = Point3(v[3]*1000, v[4]*1000, v[5]*1000);
        Point3 v2 = Point3(v[6]*1000, v[7]*1000, v[8]*1000);
        vol->addFace(Face(v0, v1, v2));
        if (fread(buffer, sizeof(uint16_t), 1, f) != 1)
        {
            fclose(f);
            return m;
        }
    } 
    fclose(f);
    return m;
}

Model loadModelSTL(const char* filename)
{
    Model m;
    FILE* f = fopen(filename, "r");

    if (f == NULL) {
        ostringstream oss;
        oss << "Can't open file " << filename << " for reading";
        throw oss.str();
    }

    int n;
    unsigned char buffer[4096];
    if (f == NULL)
        return m;

    if ((n = fread(buffer, 1, 4096, f)) < 1)
    {
        fclose(f);
        return m;
    }
    fclose(f);

    if (strncasecmp((char*)buffer, "SOLID", 5) != 0)
    {
        return loadModelSTL_binary(filename);
    } else {
        int printable = 0;
        for (int i=0; i<n; i++) {
            if (buffer[i]<127) {
                printable++;
            }
        }
        if (printable/(float)n < 0.95) {
            return loadModelSTL_binary(filename);
        } else {
            return loadModelSTL_ascii(filename);
        }
    }
}

Model loadModelFromFile(const char* filename)
{
    Model m;
    const char* ext = strrchr(filename, '.');
    if (ext && strcasecmp(ext, ".stl") == 0)
    {
        return loadModelSTL(filename);
    }
    if (filename[0] == '#' && binaryMeshBlob != NULL)
    {
        while(*filename == '#')
        {
            filename++;

            m.volumes.push_back(Volume());
            Volume* vol = &m.volumes[m.volumes.size()-1];
            int32_t n, pNr = 0;
            if (fread(&n, 1, sizeof(int32_t), binaryMeshBlob) < 1)
                return m;
            //log("Reading mesh from binary blob with %i vertexes\n", n);
            Point3 v[3];
            while(n)
            {
                float f[3];
                if (fread(f, 3, sizeof(float), binaryMeshBlob) < 1)
                    return m;
                Point3 fp(f[0]*1000, f[1]*1000, f[2]*1000);
                v[pNr++] = fp;
                if (pNr == 3)
                {
                    vol->addFace(Face(v[0], v[1], v[2]));
                    pNr = 0;
                }
                n--;
            }
        }
        return m;
    }
    return m;
}
