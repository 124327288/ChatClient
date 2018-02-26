#include "signatureprotocol.h"
#include <QDebug>
#include "expdef.h"
void SignatureProtocol::UnMarshal()
{
    uint16 len = GetUInt16();
    sig = GetString(len);
}

QString SignatureProtocol::getSig() const
{
    return sig;
}

void SignatureProtocol::setSig(const QString &value)
{
    sig = value;
}
