#include "timtool.h"
#include "timcallback.h"
#include "loginwindow.h"
#include "tcpsocket.h"
#include "C2S/userpwdprotocol.h"
#include <QTimer>
#include <Windows.h>

TimTool::TimTool() :
    sdk_app_id(1400067035),
    str_app_id("1400067035"),
    account_type("22342"),
    private_key("-----BEGIN PRIVATE KEY-----MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgySwtuhI1jDkKvsN71WD/0sNsrT7WlMT64+pk1B7OCqShRANCAASm3KZvAY2ZOh2yLU5suIYjYS1EsKZDz6lboryFZUFMt8HiBb0wPH+vH1law55/q6Imlf9k/73TGHorb4wMONhm-----END PRIVATE KEY-----"),
    timPath("./TimPath")
{
    connect(this, &TimTool::NewMsg, this, &TimTool::NewMsgHandler);
    connect(this, &TimTool::GetSignature, [=](QString _sig){
        sig = _sig;
        emit SetSignatureSuccess();
    });
    connect(this, &TimTool::SetSignatureSuccess, this, &TimTool::_Login);
}

TimTool &TimTool::Instance()
{
    static TimTool instance;
    return instance;
}

void TimTool::Init()
{
    TIMSetMode(1);
    if(!TimPathExist())
        MakeTimPath();
    TIMSetPath(timPath.data());
    TIMSetLogLevel(TIMLogLevel::kLogDebug);
    SetConnCallBack();
    TIMInit();
}

void TimTool::SetConnCallBack()
{
    static TIMConnCB cb;
    cb.OnConnected = onConnected;
    cb.OnDisconnected = onDisconnected;
    cb.data = &cb;
    TIMSetConnCallBack(&cb);
}

void TimTool::SetMessageCallback()
{
    static TIMMessageCB cb;
    cb.OnNewMessage = onGetNewMessage;
    cb.data = &cb;
    TIMSetMessageCallBack(&cb);
}

bool TimTool::TimPathExist() const
{
    QDir d(timPath.data());
    return d.exists();
}

void TimTool::MakeTimPath()
{
    QDir d;
    d.mkdir(timPath.data());
}

void TimTool::_Login()
{
    TIMUserInfo user;
    user.account_type =  const_cast<char*>(account_type.data());
    user.app_id_at_3rd = const_cast<char*>(str_app_id.data());
    QByteArray bytes = id.toUtf8();
    qDebug() << "id: " << bytes.data();
    user.identifier = bytes.data();
    QByteArray sigByte = sig.toUtf8();
    static TIMCommCB cb;
    cb.OnSuccess = &onLoginSuccess;
    cb.OnError = &onLoginError;
    cb.data = &cb;
    TIMLogin(sdk_app_id, &user, sigByte.data(), &cb);
}

void TimTool::Login(const QString &username, const QString &password)
{
    id = username;
    pwd = password;
    TcpSocket::Instance().TryConnect();
    UserPwdProtocol prc;
    prc.setUsername(username);
    prc.setPassword(password);
    prc.SendData();

//    QTimer *timer200 = new QTimer();
//    timer200->setInterval(200);
//    QTimer *timer5000 = new QTimer();
//    timer5000->setInterval(5000);
//    timer5000->setSingleShot(true);
//    QObject::connect(timer200, &QTimer::timeout, [=]{
//        emit LoginWindow::Instance().RemainTime(timer5000->remainingTime());
//        if(!sig.isEmpty())
//        {
//            TIMUserInfo user;
//            user.account_type =  const_cast<char*>(account_type.data());
//            user.app_id_at_3rd = const_cast<char*>(str_app_id.data());
//            QByteArray bytes = username.toLatin1();
//            user.identifier = bytes.data();
//            QByteArray sigByte = sig.toLatin1();
//            static TIMCommCB cb;
//            cb.OnSuccess = &onLoginSuccess;
//            cb.OnError = &onLoginError;
//            cb.data = &cb;
//            TIMLogin(sdk_app_id, &user, sigByte.data(), &cb);
//            timer200->stop();
//        }
//    });
//    QObject::connect(timer5000, &QTimer::timeout, [=]{
//        emit LoginWindow::Instance().RemainTime(0);
//        if(timer200->isActive())
//            timer200->stop();
//    });
//    timer5000->start();
//    timer200->start();
}

void TimTool::GetFriendList()
{
    static TIMGetFriendListCallback cb;
    cb.OnSuccess = onGetFriendListSuccess;
    cb.OnError = onGetFriendListError;
    cb.data = &cb;
    TIMGetFriendList(&cb);
}

void TimTool::AddSingleFriend(QString id, QString nick, QString remark, QString addWord, QString addSource)
{
    QByteArray bId = id.toUtf8();
    QByteArray bNick = nick.toUtf8();
    QByteArray bRemark = remark.toUtf8();
    QByteArray bAddSource = addSource.toUtf8();
    QByteArray bAddWord = addWord.toUtf8();
    TIMAddFriendHandle handle = CreateAddFriendHandle();
    SetID4AddFriendHandle(handle, bId.data());
    SetNickName4AddFriendHandle(handle, bNick.data(), nick.size());
    SetRemark4AddFriendHandle(handle, bRemark.data(), remark.size());
    SetAddSource4AddFriendHandle(handle, bAddSource.data());
    SetAddWord4AddFriendHandle(handle, bAddWord.data(), addWord.size());

    static TIMAddFriendCallback cb;
    cb.OnSuccess = onAddFriendSuccess;
    cb.OnError = onAddFriendError;
    cb.data = &cb;
    TIMAddFriend(&handle, 1, &cb);
    Sleep(1);
    DestroyAddFriendHandle(handle);
}

void TimTool::GetSelfProfile()
{
    static TIMGetSelfProfileCallback cb;
    cb.OnSuccess = onGetSelfProfileSuccess;
    cb.OnError = onGetSelfProfileError;
    cb.data = &cb;
    TIMGetSelfProfile(&cb);
}

void TimTool::SendMsg(QString id, QString text)
{
    //    void SendMsg(TIMConversationHandle conv_handle, TIMMessageHandle msg_handle, TIMCommCB *callback);
    //    typedef void* TIMMsgTextElemHandle;
    //    TIMMsgTextElemHandle    CreateMsgTextElem();

    //    void    SetContent(TIMMsgTextElemHandle handle, const char* content);
    //    uint32_t    GetContentLen(TIMMsgTextElemHandle handle);
    //    int    GetContent(TIMMsgTextElemHandle handle, char* content, uint32_t* len);
    TIMMessageHandle msgHandle = CreateTIMMessage();
    TIMMsgTextElemHandle txtHandle = CreateMsgTextElem();
    static QByteArray bytes;
    bytes = text.toUtf8();
    SetContent(txtHandle, bytes.data());
    AddElem(msgHandle, txtHandle);
    static TIMCommCB cb;
    cb.OnSuccess = onCommSuccess;
    cb.OnError = onCommError;
    cb.data = &cb;
    ::SendMsg(convMap[id], msgHandle, &cb);
    Sleep(1);
    DestroyElem(txtHandle);
    DestroyTIMMessage(msgHandle);
}

int TimTool::AddChatWindowMap(QString id, ChatWindow *window)
{
    if(chatWindowMap.contains(id))
        return 1;
    chatWindowMap[id] = window;
    return 0;
}

void TimTool::UpdateChatWindowMap(QString id, ChatWindow *window)
{
    chatWindowMap[id] = window;
}

int TimTool::RemoveChatWindowMap(QString id)
{
    if(!chatWindowMap.contains(id))
        return 1;
    chatWindowMap.remove(id);
    return 0;
}

ChatWindow *TimTool::GetChatWindow(QString id)
{
    if(chatWindowMap.contains(id))
        return chatWindowMap.value(id);
    return nullptr;
}

bool TimTool::ContainInChatWindowMap(QString id)
{
    return chatWindowMap.contains(id);
}

int TimTool::AddConvMap(QString id, TIMConversationHandle handle)
{
    if(convMap.contains(id))
        return 1;
    convMap[id] = handle;
    return 0;
}

void TimTool::UpdateConvMap(QString id, TIMConversationHandle handle)
{
    convMap[id] = handle;
}

int TimTool::RemoveConvMap(QString id)
{
    if(!convMap.contains(id))
        return 1;
    convMap.remove(id);
    return 0;
}

TIMConversationHandle TimTool::GetConvHandle(QString id)
{
    if(convMap.contains(id))
        return convMap.value(id);
    return nullptr;
}

bool TimTool::ContainInConvMap(QString id)
{
    return convMap.contains(id);
}

QVector<ChatContentEX> TimTool::GetContentEX(QString id)
{
    return contentMap[id];
}

void TimTool::ClearContentEX()
{
    contentMap.clear();
}

void TimTool::NewMsgHandler(QString id, QString nick, uint32_t time, QString msg)
{
    qDebug() << "chatMap.contains(id): " << chatWindowMap.contains(id);
    if(chatWindowMap.contains(id))
    {
        ChatWindow *window = chatWindowMap[id];
        window->AddContent(id, nick, time, msg);
    }
    else
    {
        contentMap[id] += { time, msg };
    }
}

QString TimTool::getNick() const
{
    return nick;
}

QString TimTool::getPwd() const
{
    return pwd;
}

QString TimTool::getId() const
{
    return id;
}

QString TimTool::getSig() const
{
    return sig;
}
