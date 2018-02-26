#ifndef SIGTHREAD_H
#define SIGTHREAD_H
#include <QThread>
class SigThread : public QThread
{
    Q_OBJECT
public:


    // QThread interface
protected:
    void run() override
    {

    }
};
#endif // SIGTHREAD_H
