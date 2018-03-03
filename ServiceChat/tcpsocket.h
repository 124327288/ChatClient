#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include "stdafx.h"
#include <QtNetwork>

#define ListenCallBack(prcClassName)    \
    prc = new prcClassName(bytes);      \
    prc->UnMarshal();                   \
    On##prcClassName(prc);

#define PrcDynamicCast(prcClassName)                            \
    prcClassName *castPrc = dynamic_cast<prcClassName*>(prc);   \
    if(castPrc == nullptr) return;


class S2CProtocol;
class TcpSocket : public QObject
{
    Q_OBJECT
private:
    TcpSocket();
public:
    static TcpSocket &Instance();
    void TryConnect();
    void Listen();
    QTcpSocket *getSocket() const;
    ~TcpSocket();
private:
    QTcpSocket *socket;
    std::string serverAddress;
    int port;

    void OnSignatureProtocol(S2CProtocol *prc);
    void OnLoginResProtocol(S2CProtocol *prc);
};
#endif // TCPSOCKET_H
