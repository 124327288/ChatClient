#include "tcpsocket.h"
#include "S2C/signatureprotocol.h"
#include "S2C/loginresprotocol.h"
#include "Tim/timtool.h"
#include "luatool.h"
#include "program.h"
#include <QTimer>
#include <QObject>
#include <signal.h>

TcpSocket::TcpSocket():
    socket(new QTcpSocket(this)),
    serverAddress(LuaTool::Instance().getServerAddress()),
    port(LuaTool::Instance().getPort())
{
    timer = new QTimer(this);
    timer->setInterval(200);
    timer->start();
    Listen();
//    QObject::connect(socket, &QTcpSocket::connected, this, &TcpSocket::Listen);
}

TcpSocket &TcpSocket::Instance()
{
    static TcpSocket instance;
    return instance;
}

void TcpSocket::TryConnect()
{
    if(socket->state() != QTcpSocket::ConnectedState)
        socket->connectToHost(QHostAddress(serverAddress.data()), port);
}

void TcpSocket::Listen()
{
    QObject::connect(timer, &QTimer::timeout, [=]{
        QByteArray bytes = socket->readAll();

        if(!bytes.isEmpty())
        {
            DEBUG_VAR(bytes.size());
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
}

void TcpSocket::OnSignatureProtocol(S2CProtocol *prc)
{
    PrcDynamicCast(SignatureProtocol);
    qDebug() << castPrc->getSig();
    emit TimTool::Instance().GetSignature(castPrc->getSig());
}

void TcpSocket::OnLoginResProtocol(S2CProtocol *prc)
{
    PrcDynamicCast(LoginResProtocol);
    switch (castPrc->getRes()) {
    case LOGINRESTYPE::SUCCESS:
        qDebug() << "LOGINRESTYPE::SUCCESS";
        break;
    case LOGINRESTYPE::FAIL:
        qDebug() << "LOGINRESTYPE::ERROR";
        emit Signal::Instance().SelfLogin(LOGINRESTYPE::FAIL);
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
    timer->stop();
}

