#ifndef LOGINRESPROTOCOL_H
#define LOGINRESPROTOCOL_H
#include "s2cprotocol.h"

class LoginResProtocol : public S2CProtocol
{
public:
    LoginResProtocol(QByteArray buffer) : S2CProtocol(PROTOCOLTYPE::LOGINRES, buffer)
    {

    }

    // S2CProtocol interface
public:
    void UnMarshal() override;
    LOGINRESTYPE getRes() const;
    void setRes(const LOGINRESTYPE &value);

private:
    LOGINRESTYPE res;
};

#endif // LOGINRESPROTOCOL_H
