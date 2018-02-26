#include "C2S/c2sprotocol.h"
#include "tcpsocket.h"

void C2SProtocol::SendData()
{
    Marshal();
    _SendData();
}

void C2SProtocol::_SendData()
{
    TcpSocket::Instance().getSocket()->write(buffer);
}
