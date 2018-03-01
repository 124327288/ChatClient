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

void TimTool::GetFriendList()
{
    //  TIM_DECL int GetID4FriendListElemHandle(TIMFriendListElemHandle handle, char* id, uint32_t* len);
    //	TIM_DECL int GetNickName4FriendListElemHandle(TIMFriendListElemHandle handle, char* buf, uint32_t* len);
    //	TIM_DECL int GetRemark4FriendListElemHandle(TIMFriendListElemHandle handle, char* remark, uint32_t* len);
    //	TIM_DECL int GetFaceURL4FriendListElemHandle(TIMFriendListElemHandle handle, char* face_url, uint32_t* len);
    TIMGetFriendListCallback cb;
    cb.OnSuccess = [](TIMFriendListElemHandle *handles, uint32_t num, void *data){
        qDebug() << "onGetFriendListSuccess";
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
    //        qDebug() << QString("id : %1, nick : %2, remark : %3").arg(sId).arg(sNick).arg(sRemark);
            friendList += {sId, sNick, sRemark};
        }
        emit TimTool::Instance().GetFriendListSuccess(friendList);
    };
    cb.OnError = [](int code, const char *desc, void *data){
        qDebug() << QString("onGetFriendListError code = %1, desc = %2").arg(code).arg(desc);
    };
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

    TIMAddFriendCallback cb;
    cb.OnSuccess = [](TIMFriendResultHandle* handles, uint32_t num, void* data){
        qDebug() << "AddSingleFriend " "OnSuccess";
        TimTool::Instance().GetFriendList();
    };
    cb.OnError = [](int code, const char* desc, void* data){
        qDebug() << "AddSingleFriend " "OnError";
        qDebug() << code << desc;
    };
    TIMAddFriend(&handle, 1, &cb);
}

void TimTool::GetSelfProfile()
{
//  typedef void* TIMSelfProfileHandle;
//	TIM_DECL TIMSelfProfileHandle CloneSelfProfileHandle(TIMSelfProfileHandle handle);
//	TIM_DECL void DestroySelfProfileHandle(TIMSelfProfileHandle handle);
//	TIM_DECL int GetNickName4SlefProfileHandle(TIMSelfProfileHandle handle, char* buf, uint32_t* len);
//	TIM_DECL E_TIMFriendAllowType GetAllowType4SlefProfileHandle(TIMSelfProfileHandle handle);

    TIMGetSelfProfileCallback cb;
    cb.OnSuccess = [](TIMSelfProfileHandle* handles, uint32_t num, void* data){
        qDebug() << "GetSelfProfile num: " << num;
        char nick[16];
        uint32_t len;
        GetNickName4SlefProfileHandle(*handles, nick, &len);
//        for(uint32_t i = 0; i < num; ++i)
//            DestroySelfProfileHandle(handles[i]);
        QString sNick = QString::fromLatin1(nick);
        emit TimTool::Instance().GetSelfNickname(sNick);
    };
    cb.OnError = [](int code, const char* desc, void* data){
        qDebug() << QString("GetSelfProfile Error! code = %1, desc = %2").arg(code).arg(desc);
    };
    TIMGetSelfProfile(&cb);
}

void TimTool::SetMessageCallback()
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
    static TIMMessageCB cb;
    cb.OnNewMessage = [](TIMMessageHandle* handles, uint32_t msg_num, void* data){
        qDebug() << "OnNewMessage";
        for(uint32_t i = 0; i < msg_num; ++i)
        {
            qDebug() << QString("msg_num : %1").arg(msg_num);
            TIMMessageHandle handle = handles[i];
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
                    QString s = QString::fromLatin1(buffer, len+1);
                    qDebug() << s;
                    delete[] buffer;
                }
                    break;
                default:
                    break;
                }
            }
        }
    };
    TIMSetMessageCallBack(&cb);
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
    TimTool::Instance().GetSelfProfile();
    emit TimTool::Instance().LoginSuccess();
}

void onLoginError(int code, const char *desc, void *data)
{
    qDebug() << QString("Error! code = %1 desc = %2").arg(code).arg(desc);
    QString str = desc;
    emit TimTool::Instance().LoginError(code, str);
}
