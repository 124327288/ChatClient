#include "timtool.h"
#include <QDebug>
#include "luatool.h"
#include "loginwindow.h"
#include "C2S/userpwdprotocol.h"
#include "tcpsocket.h"
#include <QTimer>
#include <Windows.h>
TimTool &TimTool::Instance()
{
    static TimTool instance;
    return instance;
}

TimTool::TimTool() :
    sdk_app_id(1400067035),
    str_app_id("1400067035"),
    account_type("22342"),
    private_key("-----BEGIN PRIVATE KEY-----MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgySwtuhI1jDkKvsN71WD/0sNsrT7WlMT64+pk1B7OCqShRANCAASm3KZvAY2ZOh2yLU5suIYjYS1EsKZDz6lboryFZUFMt8HiBb0wPH+vH1law55/q6Imlf9k/73TGHorb4wMONhm-----END PRIVATE KEY-----"),
    timPath("./TimPath")
{
    connect(this, &TimTool::NewMsg, this, &TimTool::NewMsgHandler);
}

bool TimTool::TimPathExist() const
{
    QDir dir(timPath.data());
    return dir.exists();
}

void TimTool::MakeTimPath()
{
    QDir d;
    d.mkdir(timPath.data());
}

void TimTool::GetFriendList()
{
    static TIMGetFriendListCallback cb;
    cb.OnSuccess = onGetFriendListSuccess;
    cb.OnError = onGetFriendListError;
    TIMGetFriendList(&cb);
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

    static TIMAddFriendCallback cb;
    cb.OnSuccess = onAddFriendSuccess;
    cb.OnError = onAddFriendError;
    TIMAddFriend(&handle, 1, &cb);
    Sleep(1);
    DestroyAddFriendHandle(handle);
}

void TimTool::GetSelfProfile()
{
    static TIMGetSelfProfileCallback cb;
    cb.OnSuccess = onGetSelfProfileSuccess;
    cb.OnError = onGetSelfProfileError;
    TIMGetSelfProfile(&cb);
}

void TimTool::SetMessageCallback()
{
    static TIMMessageCB cb;
    cb.OnNewMessage = onNewMessage;
    TIMSetMessageCallBack(&cb);
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
    bytes = text.toLatin1();
    qDebug() << "send: " << bytes.data();
    SetContent(txtHandle, "bytes.data()");
    AddElem(msgHandle, txtHandle);
    qDebug() << "send: " << bytes.data();
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

void TimTool::NewMsgHandler(QString id, QString nick, uint32_t time, QString msg)
{
    qDebug() << "chatMap.contains(id): " << chatWindowMap.contains(id);
    if(chatWindowMap.contains(id))
    {
        ChatWindow *window = chatWindowMap[id];
        window->AddContent(id, nick, time, msg);
    }
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
    id = username;
    pwd = password;
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
            static TIMCommCB cb;
            cb.OnSuccess = &onCommSuccess;
            cb.OnError = &onCommError;
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

void onErrorDebug(QString name, int code, const char *desc)
{
    qDebug() << QString("[%1] code = %2, desc = %3").arg(name).arg(code).arg(desc);
}

#define ERROR_DEBUG onErrorDebug(__func__, code, desc)
void onCommSuccess(void *data)
{
    TimTool::Instance().GetSelfProfile();
    emit TimTool::Instance().LoginSuccess();
}

void onCommError(int code, const char *desc, void *data)
{
    ERROR_DEBUG;
    QString str = desc;
    emit TimTool::Instance().LoginError(code, str);
}

void onGetFriendListSuccess(TIMFriendListElemHandle *handles, uint32_t num, void *data)
{
    //  TIM_DECL int GetID4FriendListElemHandle(TIMFriendListElemHandle handle, char* id, uint32_t* len);
    //	TIM_DECL int GetNickName4FriendListElemHandle(TIMFriendListElemHandle handle, char* buf, uint32_t* len);
    //	TIM_DECL int GetRemark4FriendListElemHandle(TIMFriendListElemHandle handle, char* remark, uint32_t* len);
    //	TIM_DECL int GetFaceURL4FriendListElemHandle(TIMFriendListElemHandle handle, char* face_url, uint32_t* len);
    QList<Linkman> friendList;
    for(uint32_t i = 0; i < num; ++i)
    {
        TIMFriendListElemHandle handle = handles[i];
        char id[16];
        char nickName[16];
        char remark[16];
        uint32_t idLen, nickLen, remarkLen;
        GetID4FriendListElemHandle(handle, id, &idLen);
        GetNickName4FriendListElemHandle(handle, nickName, &nickLen);
        GetRemark4FriendListElemHandle(handle, remark, &remarkLen);
        QString sId = QString::fromLatin1(id, idLen);
        QString sNick = QString::fromLatin1(nickName, nickLen);
        QString sRemark = QString::fromLatin1(remark, remarkLen);
        friendList += {sId, sNick, sRemark};
    }
    emit TimTool::Instance().GetFriendListSuccess(friendList);
}

void onGetFriendListError(int code, const char *desc, void *data)
{
    ERROR_DEBUG;
}

void onAddFriendSuccess(TIMFriendResultHandle *handles, uint32_t num, void *data)
{
    TimTool::Instance().GetFriendList();
}

void onAddFriendError(int code, const char *desc, void *data)
{
    onErrorDebug(__func__, code, desc);
}

void TimTool::setPwd(const QString &value)
{
    pwd = value;
}

void TimTool::setId(const QString &value)
{
    id = value;
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

void TimTool::setSig(const QString &value)
{
    sig = value;
}

void onGetSelfProfileSuccess(TIMSelfProfileHandle *handles, uint32_t num, void *data)
{
    //  typedef void* TIMSelfProfileHandle;
    //	TIM_DECL TIMSelfProfileHandle CloneSelfProfileHandle(TIMSelfProfileHandle handle);
    //	TIM_DECL void DestroySelfProfileHandle(TIMSelfProfileHandle handle);
    //	TIM_DECL int GetNickName4SlefProfileHandle(TIMSelfProfileHandle handle, char* buf, uint32_t* len);
    //	TIM_DECL E_TIMFriendAllowType GetAllowType4SlefProfileHandle(TIMSelfProfileHandle handle);

    qDebug() << "GetSelfProfile num: " << num;
    char nick[16];
    uint32_t len;
    GetNickName4SlefProfileHandle(*handles, nick, &len);
    QString sNick = QString::fromLatin1(nick);
    emit TimTool::Instance().GetSelfNickname(sNick);
}

void onGetSelfProfileError(int code, const char *desc, void *data)
{
    onErrorDebug(__func__, code, desc);
}

void onNewMessage(TIMMessageHandle *handles, uint32_t msg_num, void *data)
{
    //    /**
    //    Description:    设置用户新消息回调函数
    //    @param    [in]    callback    新消息回调函数
    //    @return            void
    //    @exception      none
    //    */
    //    TIM_DECL void            TIMSetMessageCallBack(TIMMessageCB *callback);

    //        /**
    //    Description:    用户新消息回调函数
    //    @param    [in]    handles        TIMMessageHandle 数组指针
    //    @param    [in]    msg_num        TIMMessageHandle 数组大小
    //    @param    [in]    data        用户自定义数据
    //    @return            void
    //    @exception      none
    //    */
    //    typedef void (*CBOnNewMessage) (TIMMessageHandle* handles, uint32_t msg_num, void* data);
    //    typedef struct _TIMMessageCB_C
    //    {
    //        CBOnNewMessage OnNewMessage;
    //        void* data;
    //    }TIMMessageCB;

    //    /**
    //      Description:    获取Message中包含的elem个数
    //      @param    [in]    handle        消息句柄
    //      @return            int            elem个数
    //      @exception      none
    //      */
    //      TIM_DECL int                GetElemCount(TIMMessageHandle handle);
    //      /**
    //      Description:    获取Message中包含的指定elem句柄
    //      @param    [in]    handle        消息句柄
    //      @param    [in]    index        elem索引
    //      @return            TIMMsgElemHandle    elem句柄
    //      @exception      none
    //      */
    //      TIM_DECL TIMMsgElemHandle    GetElem(TIMMessageHandle handle, int index);
    //    /**
    //    Description:    当前消息的时间戳
    //    @param    [in]    handle    TIMMessageHandle
    //    @return            uint32_t    时间戳
    //    @exception      none
    //    */
    //    TIM_DECL uint32_t            GetMsgTime(TIMMessageHandle handle);
    //    /**
    //       Description:    获取发送方
    //       @param    [in]    handle    TIMMessageHandle
    //       @param    [in]    buf        发送方ID buffer
    //       @param    [in]    len        发送方ID 长
    //       @return            int        0:成功 非0:失败
    //       @exception      none
    //       */
    //       TIM_DECL int                GetMsgSender(TIMMessageHandle handle, char* buf, uint32_t* len);
    //       /**
    //       Description:    获取发送者资料
    //       @param    [in]    handle    TIMMessageHandle
    //       @param    [in]    profile    发送者资料 目前只有字段：identifier、nickname、faceURL、customInfo
    //       @return            int        0:成功 非0:失败
    //       @exception      none
    //       */
    //       TIM_DECL int                GetSenderProfile(TIMMessageHandle handle, TIMProfileHandle profile);

    qDebug() << QString("OnNewMessage msg num : %1").arg(msg_num);
    for(uint32_t i = 0; i < msg_num; ++i)
    {
        TIMMessageHandle handle = handles[i];
        uint32_t msgTime = GetMsgTime(handle);

        TIMProfileHandle profile = CreateProfileHandle();
        GetSenderProfile(handle, profile);

        char id[16];uint32_t idLen;
        GetID4ProfileHandle(profile, id, &idLen);
        QString sid = QString::fromLatin1(id, idLen);

        char nick[16];uint32_t nickLen;
        GetNickName4ProfileHandle(profile, nick, &nickLen);
        QString snick = QString::fromLatin1(nick, nickLen);

        QString msg;
        int cnt = GetElemCount(handle);
        for(int j = 0; j < cnt; ++j)
        {
            TIMMsgElemHandle elem = GetElem(handle, j);
            TIMElemType type = GetElemType(elem);
            switch (type) {
            case TIMElemType::kElemText:
            {
                uint32_t len = GetContentLen(elem);
                char *buffer = new char[len + 1];
                GetContent(elem, buffer, &len);
                QString s = QString::fromLatin1(buffer, len + 1);
                msg += s;
                qDebug() << buffer;
                delete[] buffer;
            }
                break;
            default:
                break;
            }
        }

        emit TimTool::Instance().NewMsg(sid, snick, msgTime, msg);
        DestroyProfileHandle(profile);
    }
}
