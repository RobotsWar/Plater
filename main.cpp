#include <iostream>
#include <fcntl.h>
#include "modelFile/modelFile.h"
#include "Request.h"
#include "Triangle.h"

using namespace std;
using namespace Plater;

int main(int argc, char *argv[])
{
    Request request;
    request.readFromStdin();
    request.process();

    return EXIT_SUCCESS;
}
