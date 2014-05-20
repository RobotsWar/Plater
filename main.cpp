#include <iostream>
#include <fcntl.h>
#include "modelFile/modelFile.h"
#include "Request.h"
#include "util.h"
#include "log.h"

using namespace std;
using namespace Plater;

void help()
{
    cerr << "Plater v1.0 (https://github.com/RobotsWar/Plater)" << endl;
    cerr << "Usage: plater [options] (will read config from stdin)" << endl;
    cerr << endl;
    cerr << "-v: Verbose mode" << endl;
    cerr << "-j precision: Sets the precision (in mm, default: 0.5)" << endl;
    cerr << "-s spacing: Change the spacing between parts (in mm, default: 2.2)" << endl;
    cerr << "-d delta: Sets the interval of place grid (in mm, default: 2)" << endl;
    cerr << "-r rotation: Sets the interval of rotation (in °, default: 90)" << endl;
    cerr << "-o pattern: output file pattern (default: plate_%03d)" << endl;
    cerr << "-p: will output ppm of the plates" << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int index;
    Request request;

    while ((index = getopt(argc, argv, "hvs:d:r:pj:")) != -1) {
        switch (index) {
            case 'h':
                help();
                break;
            case 'v':
                increaseVerboseLevel();
                break;
            case 's':
                request.spacing = atof(optarg)*1000;
                break;
            case 'd':
                request.delta = atof(optarg)*1000;
                break;
            case 'r':
                request.deltaR = DEG2RAD(atof(optarg));
                break;
            case 'p':
                request.mode = REQUEST_PPM;
                break;
            case 'j':
                request.precision = atof(optarg)*1000;
                break;
        }
    }

    request.readFromStdin();
    request.process();

    return EXIT_SUCCESS;
:}
