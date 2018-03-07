#ifndef S2CPROTOCOL_H
#define S2CPROTOCOL_H
#include <QtNetwork>
#include "exptype.h"

class S2CProtocol
{
protected:
    PROTOCOLTYPE type;
    QByteArray buffer;
private:
    int cur;
public:
    S2CProtocol(PROTOCOLTYPE type, QByteArray buffer):type(type), buffer(buffer)
    {
        cur = 1;
    }
    virtual void UnMarshal() = 0;

    byte GetByte()
    {
        return buffer.at(cur++);
    }

    uint16 GetUInt16()
    {
        int i = cur;
        cur += 2;
        return *(uint16*)(buffer.data() + i);
    }

    QString GetString(int len)
    {
        int i = cur;
        cur += len;
        return buffer.mid(i, len);
    }
};

#endif // S2CPROTOCOL_H
