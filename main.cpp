#include <iostream>
#include <fcntl.h>
#include "modelFile/modelFile.h"

using namespace std;

int main(int argc, char *argv[])
{
    string file;
    cin >> file;

    FMatrix3x3 id;
    SimpleModel *stl = loadModelFromFile(file.c_str(), id);
    Point3 pt = stl->max();
    cout << pt.x << ", " << pt.y << ", " << pt.z << endl;
    delete stl;
}
