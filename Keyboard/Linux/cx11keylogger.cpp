#include "cx11keylogger.h"
#include <cx11keyloggerworker.h>

CX11Keylogger::CX11Keylogger(QObject *parent) : QObject(parent)
{
    CX11KeyloggerWorker *worker = new CX11KeyloggerWorker(&workingThread_);
    worker->moveToThread(&workingThread_);
    connect(&workingThread_, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &CX11Keylogger::Start, worker, &CX11KeyloggerWorker::StartLogging);
    workingThread_.start();
    emit Start();
}

CX11Keylogger::~CX11Keylogger() {
    workingThread_.requestInterruption();
    workingThread_.exit(); // this doesn't work until some key is pressed. It can be fixed with terminate(), but it' quite bad method
    workingThread_.wait();
}

