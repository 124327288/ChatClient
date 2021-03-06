﻿#include "timtool.h"
#include "timcallback.h"
#include "loginwindow.h"
#include "mainwindow.h"
#include "Protocol/tcpsocket.h"
#include "Protocol/C2S/userpwdprotocol.h"
#include "chatmanager.h"
#include <QTimer>
#include <fstream>
#include <Windows.h>

TimTool::TimTool() :
    sdk_app_id(1400067035),
    str_app_id("1400067035"),
    account_type("22342"),
    private_key("-----BEGIN PRIVATE KEY-----MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgySwtuhI1jDkKvsN71WD/0sNsrT7WlMT64+pk1B7OCqShRANCAASm3KZvAY2ZOh2yLU5suIYjYS1EsKZDz6lboryFZUFMt8HiBb0wPH+vH1law55/q6Imlf9k/73TGHorb4wMONhm-----END PRIVATE KEY-----"),
    timPath("./TimPath")
{
    connect(this, &TimTool::OnKickOffline, this, &TimTool::OnKickOfflineHandle);
    connect(this, &TimTool::NewMsg, this, &TimTool::NewMsgHandler);
    connect(this, &TimTool::GetFileSuccess, this, &TimTool::GetFileSuccessHandle);
    connect(this, &TimTool::GetFriendListSuccess, this, &TimTool::GetFriendListSuccessHandle);
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
    SetMessageCallback();
    SetKickOfflineCallBack();
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

void TimTool::SetKickOfflineCallBack()
{
    static TIMForceOfflineCB cb;
    cb.OnKickOffline = &onKickOffline;
    cb.data = &cb;
    TIMSetKickOfflineCallBack(&cb);
}

void TimTool::GetLocalMessage()
{

}

void TimTool::Logout()
{
    TIMCommCB cb;
    cb.OnSuccess = &onLogoutSuccess;
    cb.OnError = &onLogoutError;
    TIMLogout(&cb);
    Sleep(1);
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

void TimTool::SetNickName(const QString &nick)
{
    QByteArray arr = nick.toUtf8();
    static TIMCommCB cb;
    cb.OnSuccess = onSetNickNameSuccess;
    cb.OnError = onSetNickNameError;
    cb.data = &cb;
    TIMSetNickName(arr.data(), arr.size(), &cb);
}

void TimTool::SendMsg(const QString &id, const QVector<TimMsg> &msgList)
{
    TIMMessageHandle msgHandle = CreateTIMMessage();
    TIMCommCB cb;
    cb.OnSuccess = &onSendMsgSuccess;
    cb.OnError = &onSendMsgError;
    QVector<TIMMsgTextElemHandle>  txtHandleList;
    QVector<TIMMsgImageElemHandle> imgHandleList;
    QVector<TIMMsgFileElemHandle>  fileHandleList;

    for(const TimMsg &msg : msgList)
    {
        switch (msg.type)
        {
        case kElemText:
        {
            TIMMsgTextElemHandle txtHandle = CreateMsgTextElem();
            QByteArray bytes = msg.msg.toUtf8();
            SetContent(txtHandle, bytes.data());
            ON_INVOKE(AddElem, msgHandle, txtHandle);
            txtHandleList += txtHandle;
            break;
        }
        case kElemImage:
        {
            TIMMsgImageElemHandle imgHandle = CreateMsgImageElem();
            QByteArray path = msg.msg.toUtf8();
            SetImageElemPath(imgHandle, path.data(), path.size());
            ON_INVOKE(AddElem, msgHandle, imgHandle);
            imgHandleList += imgHandle;
            break;
        }
        case kElemFile:
        {
            SendFile(id, msg.msg);
//            TIMMsgFileElemHandle fileHandle = CreateFileElemHandle();
//            QString filePath = msg.msg;
//            auto setFileName = [=]{
//                std::string stdFilePath = filePath.toStdString();
//                std::string fileName = stdFilePath.substr(stdFilePath.find_last_of('/') + 1);
//                DEBUG_VAR(fileName);
//                SetFileElemFileName(fileHandle, fileName.data(), fileName.length());
//            };

//            auto setFileData = [=]{
//                std::wstring stdFilePath = filePath.toStdWString();
//                std::fstream sendFile(stdFilePath.data(), std::fstream::in | std::fstream::binary);
//                std::string fileData((std::istreambuf_iterator<char>(sendFile)), std::istreambuf_iterator<char>());
//                SetFileElemData(fileHandle, fileData.data(), fileData.length());
//            };

//            setFileName();
//            setFileData();
//            ON_INVOKE(AddElem, msgHandle, fileHandle);
//            fileHandleList += fileHandle;
            break;
        }
        default:
            break;
        }
    }
    if (!convMap.contains(id))
    {
        convMap.insert(id, CreateConversation());
    }
    ::SendMsg(convMap[id], msgHandle, &cb);
    Sleep(1);
    for(auto handle : txtHandleList)
        DestroyElem(handle);
    for(auto handle : imgHandleList)
        DestroyElem(handle);
    for(auto handle : fileHandleList)
        DestroyElem(handle);
    DestroyTIMMessage(msgHandle);
}

void TimTool::SendMsg(QString id, QString text)
{
    TIMMessageHandle msgHandle = CreateTIMMessage();
    TIMMsgTextElemHandle txtHandle = CreateMsgTextElem();

    QByteArray bytes = text.toUtf8();
    SetContent(txtHandle, bytes.data());
    ON_INVOKE(AddElem, msgHandle, txtHandle);

    TIMCommCB cb;
    cb.OnSuccess = &onSendMsgSuccess;
    cb.OnError = &onSendMsgError;
	if (!convMap.contains(id))
	{
		convMap.insert(id, CreateConversation());
	}
    ::SendMsg(convMap[id], msgHandle, &cb);
    Sleep(1);
    DestroyElem(txtHandle);
    DestroyTIMMessage(msgHandle);
}

void TimTool::SendImage(const QString &id, const QString &imgPath)
{
    TIMMessageHandle msg = CreateTIMMessage();
    TIMMsgImageElemHandle elem = CreateMsgImageElem();

    QByteArray path = imgPath.toUtf8();
    SetImageElemPath(elem, path.data(), path.size());
    ON_INVOKE(AddElem, msg, elem);

    TIMCommCB callback;
    callback.OnSuccess = &onSendImageSuccess;
    callback.OnError = &onSendImageError;
	if (!convMap.contains(id))
	{
		convMap.insert(id, CreateConversation());
	}
    ::SendMsg(convMap[id], msg, &callback);
    Sleep(1);
    DestroyTIMMessage(msg);
    DestroyElem(elem);
}

void TimTool::SendFile(const QString &id, const QString &filePath)
{
    TIMMessageHandle msg = CreateTIMMessage();
    TIMMsgFileElemHandle elem = CreateFileElemHandle();

    auto setFileName = [=]{
        std::string stdFilePath = filePath.toStdString();
        std::string fileName = stdFilePath.substr(stdFilePath.find_last_of('/') + 1);
        DEBUG_VAR(fileName);
        SetFileElemFileName(elem, fileName.data(), fileName.length());
    };

    auto setFileData = [=]{
        std::wstring stdFilePath = filePath.toStdWString();
        std::fstream sendFile(stdFilePath.data(), std::fstream::in | std::fstream::binary);
        std::string fileData((std::istreambuf_iterator<char>(sendFile)), std::istreambuf_iterator<char>());
        SetFileElemData(elem, fileData.data(), fileData.length());
    };

    setFileName();
    setFileData();
    ON_INVOKE(AddElem, msg, elem);

    static TIMCommCB callback;
    callback.OnSuccess = &onSendFileSuccess;
    callback.OnError = &onSendFileError;
	if (!convMap.contains(id))
	{
		convMap.insert(id, CreateConversation());
	}
	::SendMsg(convMap[id], msg, &callback);
    Sleep(1);
    DestroyTIMMessage(msg);
    DestroyElem(elem);
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
    contentMap[id] += { time, msg };
    ChatManager::Instance().AddToChatMap(id, { false, time, msg});
    if(chatWindowMap.contains(id))
    {
        ChatWindow *window = chatWindowMap[id];
        window->AddContent(id, nick, time, msg);
    }
}

void TimTool::OnKickOfflineHandle(void *data)
{
    QMessageBox::information(nullptr, QObject::tr("Be Kick Offline!"),  QObject::tr("Be Kick Offline! Logout!"));
//    Restart();
    for(auto window : TimTool::Instance().getChatWindowMap())
        window->close();
    MainWindow::Instance().close();
    LoginWindow::Instance().show();
}

void TimTool::GetFileSuccessHandle(const QString &id, const QString &nick, time_t time, const QString &fileName, const QString &filePath, const QString &folderPath)
{
    DEBUG_FUNC;
    if(chatWindowMap.contains(id))
    {
        ChatWindow *window = chatWindowMap[id];
        window->AddFileDesc(id, nick, time, fileName, filePath, folderPath);
    }
}

void TimTool::GetFriendListSuccessHandle(const QList<Linkman>& friendList)
{
    this->friendList = friendList;
}

TimTool::~TimTool()
{
    for(auto handle : convMap)
    {
        DestroyConversation(handle);
    }
}

QList<Linkman> TimTool::getFriendList() const
{
    return friendList;
}

void TimTool::setFriendList(const QList<Linkman> &value)
{
    friendList = value;
}

QMap<QString, QVector<ChatContentEX> > TimTool::getContentMap() const
{
    return contentMap;
}

void TimTool::setContentMap(const QMap<QString, QVector<ChatContentEX> > &value)
{
    contentMap = value;
}

QMap<QString, TIMConversationHandle> TimTool::getConvMap() const
{
    return convMap;
}

void TimTool::setConvMap(const QMap<QString, TIMConversationHandle> &value)
{
    convMap = value;
}

QMap<QString, ChatWindow *> TimTool::getChatWindowMap() const
{
    return chatWindowMap;
}

void TimTool::setChatWindowMap(const QMap<QString, ChatWindow *> &value)
{
    chatWindowMap = value;
}

void TimTool::setNick(const QString &value)
{
    nick = value;
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
