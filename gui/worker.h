#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include "Request.h"

class Worker : public QThread
{
    Q_OBJECT

    void run();

public:
    Plater::Request request;
    Worker();
    std::string parts;

signals:
    void workerEnd();
};

#endif // WORKER_H
