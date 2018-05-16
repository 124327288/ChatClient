#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include "stdafx.h"
#include <QtNetwork>

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
    QTimer *timer;
    std::string serverAddress;
    int port;

    void OnSignatureProtocol(S2CProtocol *prc);
    void OnLoginResProtocol(S2CProtocol *prc);
    void OnRegisterRetProtocol(S2CProtocol *prc);
};
#endif // TCPSOCKET_H
