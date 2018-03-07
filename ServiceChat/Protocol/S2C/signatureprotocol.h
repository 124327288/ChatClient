#ifndef SIGNATUREPROTOCOL_H
#define SIGNATUREPROTOCOL_H
#include "s2cprotocol.h"

class SignatureProtocol : public S2CProtocol
{
public:
    SignatureProtocol(QByteArray buffer) : S2CProtocol(PROTOCOLTYPE::SIGNATURE, buffer)
    {

    }

    // S2CProtocol interface
public:
    void UnMarshal() override;
    QString getSig() const;
    void setSig(const QString &value);

private:
    QString sig;
};

#endif // SIGNATUREPROTOCOL_H
