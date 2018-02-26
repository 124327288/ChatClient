#include "loginresprotocol.h"

void LoginResProtocol::UnMarshal()
{
    res = (LOGINRESTYPE)GetByte();
}

LOGINRESTYPE LoginResProtocol::getRes() const
{
    return res;
}

void LoginResProtocol::setRes(const LOGINRESTYPE &value)
{
    res = value;
}
