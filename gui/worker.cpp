#include "worker.h"

Worker::Worker()
{
}

void Worker::run()
{
    request.cancel = false;
    request.readPartsFromString(parts);
    if (!request.hasError) {
        request.process();
    }

    emit workerEnd();
}
