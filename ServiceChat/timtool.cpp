#include "timtool.h"
#include <QDebug>
#include "luatool.h"
#include "loginwindow.h"
#include "C2S/userpwdprotocol.h"
#include "tcpsocket.h"
#include <QTimer>
TimTool &TimTool::Instance()
{
    static TimTool instance;
    return instance;
}

TimTool::TimTool() :
    sdk_app_id(1400067035),
    str_app_id("1400067035"),
    account_type("22342"),
    private_key("-----BEGIN PRIVATE KEY-----MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgySwtuhI1jDkKvsN71WD/0sNsrT7WlMT64+pk1B7OCqShRANCAASm3KZvAY2ZOh2yLU5suIYjYS1EsKZDz6lboryFZUFMt8HiBb0wPH+vH1law55/q6Imlf9k/73TGHorb4wMONhm-----END PRIVATE KEY-----")
{

}

QString TimTool::getSig() const
{
    return sig;
}

void TimTool::setSig(const QString &value)
{
    sig = value;
}

void TimTool::AddSingleFriend(QString id, QString nick, QString remark, QString addWord, QString addSource)
{
    QByteArray bId = id.toLatin1();
    QByteArray bNick = nick.toLatin1();
    QByteArray bRemark = remark.toLatin1();
    QByteArray bAddSource = addSource.toLatin1();
    QByteArray bAddWord = addWord.toLatin1();
    TIMAddFriendHandle handle = CreateAddFriendHandle();
    SetID4AddFriendHandle(handle, bId.data());
    SetNickName4AddFriendHandle(handle, bNick.data(), nick.size());
    SetRemark4AddFriendHandle(handle, bRemark.data(), remark.size());
    SetAddSource4AddFriendHandle(handle, bAddSource.data());
    SetAddWord4AddFriendHandle(handle, bAddWord.data(), addWord.size());

    TIMAddFriendCallback cb;
    cb.OnSuccess = [](TIMFriendResultHandle* handles, uint32_t num, void* data){
        qDebug() << "AddSingleFriend " "OnSuccess";
//        TIM_DECL int GetID4FriendResultHandle(TIMFriendResultHandle handle, char* id, uint32_t* len);
//        TIM_DECL uint64_t GetResult4FriendResultHandle(TIMFriendResultHandle handle);
        for(int i = 0; i < num; ++i)
        {
            char c_id[128] = {0};
            uint32_t len;
            GetID4FriendResultHandle(handles[i], c_id, &len);
            qDebug() << c_id;
        }
    };
    cb.OnError = [](int code, const char* desc, void* data){
        qDebug() << "AddSingleFriend " "OnError";
        qDebug() << code << desc;
    };

    TIMAddFriend(&handle, 1, &cb);
}

void TimTool::Init()
{
    TIMSetMode(1);
    TIMSetPath("./TimPath");
    TIMSetLogLevel(TIMLogLevel::kLogInfo);
    SetConnCallBack();
    TIMInit();
}

void TimTool::SetConnCallBack()
{
    static TIMConnCB cb;
    cb.OnConnected = [](void*){
        qDebug() << "On Connect!";
    };
    cb.OnDisconnected = [](void*){
        qDebug() << "On Disconnect!";
    };
    cb.data = &cb;
    TIMSetConnCallBack(&cb);
}

void TimTool::Login(const QString &username, const QString &password)
{
    TcpSocket::Instance().TryConnect();
    UserPwdProtocol prc;
    prc.setUsername(username);
    prc.setPassword(password);
    prc.SendData();

    QTimer *timer200 = new QTimer();
    timer200->setInterval(200);
    QTimer *timer5000 = new QTimer();
    timer5000->setInterval(5000);
    timer5000->setSingleShot(true);
    QObject::connect(timer200, &QTimer::timeout, [=]{
        emit LoginWindow::Instance().RemainTime(timer5000->remainingTime());
        if(!sig.isEmpty())
        {
            TIMUserInfo user;
            user.account_type =  const_cast<char*>(account_type.data());
            user.app_id_at_3rd = const_cast<char*>(str_app_id.data());
            QByteArray bytes = username.toLatin1();
            user.identifier = bytes.data();
            QByteArray sigByte = sig.toLatin1();
            TIMCommCB cb;
            cb.OnSuccess = &onLoginSuccess;
            cb.OnError = &onLoginError;
            TIMLogin(sdk_app_id, &user, sigByte.data(), &cb);
            timer200->stop();
        }
    });
    QObject::connect(timer5000, &QTimer::timeout, [=]{
        emit LoginWindow::Instance().RemainTime(0);
        if(timer200->isActive())
            timer200->stop();
    });
    timer5000->start();
    timer200->start();
}

void onLoginSuccess(void *data)
{
    qDebug() << "Success!";
    emit TimTool::Instance().LoginSuccess();
}

void onLoginError(int code, const char *desc, void *data)
{
    qDebug() << QString("Error! code = %1 desc = %2").arg(code).arg(desc);
    QString str = desc;
    emit TimTool::Instance().LoginError(code, str);
}
