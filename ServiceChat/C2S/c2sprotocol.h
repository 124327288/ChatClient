#ifndef C2SPROTOCOL_H
#define C2SPROTOCOL_H
#include "exptype.h"
#include <QString>
#include <QtNetwork>
#include <QByteArray>
class C2SProtocol
{
public:
    C2SProtocol(PROTOCOLTYPE type) : type(type)
    {
        Add((byte)type);
    }
    void SendData();

    template <class T>
    void Add(T t)
    {
        buffer += t;
    }
private:
    virtual void Marshal() = 0;
    void _SendData();
    PROTOCOLTYPE type;
    QByteArray buffer;
};

#endif // C2SPROTOCOL_H
