#ifndef CX11KEYLOGGERWORKER_H
#define CX11KEYLOGGERWORKER_H

#include <QObject>
#include <QThread>

// X11 is the worst thing ever made so this includes will break all following includes => It must be included LAST
#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>

class CX11KeyloggerWorker : public QObject
{
    Q_OBJECT
public:
    explicit CX11KeyloggerWorker(QThread* myThread, QObject *parent = nullptr);

signals:
    void RawKeyPressedSignal(XIRawEvent);
    void RawKeyReleasedSignal(XIRawEvent);

public slots:
    void StartLogging();

private:
    QThread *myThread_;
    Display *X11Display_;
    int xi_opcode_;
};

#endif // CX11KEYLOGGERWORKER_H
