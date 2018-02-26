#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QtNetwork>
#include <thread>
#include <QObject>
//#include "S2C/s2cprotocol.h"
class S2CProtocol;
class TcpSocket : public QObject
{
    Q_OBJECT
public:
    static TcpSocket &Instance();
    void TryConnect();
    void Listen();
    QTcpSocket *getSocket() const;
    ~TcpSocket();
private:
    TcpSocket();
    QTcpSocket *socket;
    std::string serverAddress;
    int port;

    void OnSignatureProtocol(S2CProtocol *prc);
    void OnLoginResProtocol(S2CProtocol *prc);
};

#endif // TCPSOCKET_H
