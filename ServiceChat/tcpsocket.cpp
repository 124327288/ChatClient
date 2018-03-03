#include "tcpsocket.h"
#include "S2C/signatureprotocol.h"
#include "S2C/loginresprotocol.h"
#include "timtool.h"
#include "luatool.h"
#include "program.h"
#include <QTimer>
#include <QObject>

TcpSocket::TcpSocket():
    socket(new QTcpSocket()),
    serverAddress(LuaTool::Instance().getServerAddress()),
    port(LuaTool::Instance().getPort())
{
    QObject::connect(socket, &QTcpSocket::connected, this, &TcpSocket::Listen);
}

TcpSocket &TcpSocket::Instance()
{
    static TcpSocket tcpSocket;
    return tcpSocket;
}

void TcpSocket::TryConnect()
{
    if(socket->state() != QTcpSocket::ConnectedState)
        socket->connectToHost(QHostAddress(serverAddress.data()), port);
}

void TcpSocket::Listen()
{
    QTimer *timer = new QTimer();
    timer->setInterval(200);
    QObject::connect(timer, &QTimer::timeout, [=]{
        if(Program::Instance().state == PROGRAMSTATE::EXIT)
            timer->stop();
        QByteArray bytes = socket->readAll();
        if(!bytes.isEmpty())
        {
            PROTOCOLTYPE type = (PROTOCOLTYPE) bytes.at(0);
            qDebug() << QString("Protocol Type: %1").arg(type);
            S2CProtocol *prc = nullptr;
            switch (type)
            {
            case PROTOCOLTYPE::SIGNATURE:
                ListenCallBack(SignatureProtocol);
                break;
            case PROTOCOLTYPE::LOGINRES:
                ListenCallBack(LoginResProtocol);
                break;
            default:
                break;
            }
            delete prc;
        }
    });
    timer->start();
}

void TcpSocket::OnSignatureProtocol(S2CProtocol *prc)
{
    PrcDynamicCast(SignatureProtocol);
    qDebug() << castPrc->getSig();
    TimTool::Instance().setSig(castPrc->getSig());
}

void TcpSocket::OnLoginResProtocol(S2CProtocol *prc)
{
    PrcDynamicCast(LoginResProtocol);
    switch (castPrc->getRes()) {
    case LOGINRESTYPE::SUCCESS:
        break;
    default:
        break;
    }
}

QTcpSocket *TcpSocket::getSocket() const
{
    return socket;
}

TcpSocket::~TcpSocket()
{
//    delete listenThread;
//    delete socket;
}

