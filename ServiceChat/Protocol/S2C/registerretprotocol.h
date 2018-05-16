#ifndef REGISTERRETPROTOCOL_H
#define REGISTERRETPROTOCOL_H
#include "s2cprotocol.h"

class RegisterRetProtocol : public S2CProtocol
{
public:
    RegisterRetProtocol(QByteArray buffer) : S2CProtocol(PROTOCOLTYPE::REGISTER_RET, buffer)
    {

    }

    // S2CProtocol interface
public:
    void UnMarshal() override;
    REGISTERRETTYPE res() const;
    void setRes(const REGISTERRETTYPE &res);

private:
    REGISTERRETTYPE m_res;
};

#endif // REGISTERRETPROTOCOL_H
