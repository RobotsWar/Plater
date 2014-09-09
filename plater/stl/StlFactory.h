#ifndef STLFACTORY_H
#define STLFACTORY_H

#include "Model.h"
using namespace Plater;

void saveModelToFileAscii(const char *filename, Model *model);
Model loadModelSTL_ascii(const char* filename);
void saveModelToFileBinary(const char *filename, Model *model);
Model loadModelSTL_binary(const char* filename);
Model loadModelSTL(const char* filename);
Model loadModelFromFile(const char* filename);

#endif
