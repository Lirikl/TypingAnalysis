#ifndef CX11KEYLOGGER_H
#define CX11KEYLOGGER_H

#include <QObject>
#include <QThread>

class CX11Keylogger : public QObject
{
    Q_OBJECT
public:
    explicit CX11Keylogger(QObject *parent = nullptr);
    ~CX11Keylogger();

signals:
    void Start();

public slots:

private:
    QThread workingThread_;
};

#endif // CX11KEYLOGGER_H

