#include "worker.h"

Worker::Worker()
    : working(false)
{
}

void Worker::run()
{
    working = true;
    request.cancel = false;
    request.readPartsFromString(parts);
    if (!request.hasError) {
        request.process();
    }
    working = false;

    emit workerEnd();
}
