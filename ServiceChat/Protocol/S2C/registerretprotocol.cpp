#include "registerretprotocol.h"

void RegisterRetProtocol::UnMarshal()
{
    m_res = (REGISTERRETTYPE)GetByte();
}

REGISTERRETTYPE RegisterRetProtocol::res() const
{
    return m_res;
}

void RegisterRetProtocol::setRes(const REGISTERRETTYPE &res)
{
    m_res = res;
}
