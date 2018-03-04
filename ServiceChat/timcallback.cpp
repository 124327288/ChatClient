#include "timcallback.h"
#include "timtool.h"
void onDebug(QString name)
{
    qDebug() << name;
}

void onErrorDebug(QString name, int code, const char *desc)
{
    qDebug() << QString("[%1] code = %2, desc = %3").arg(name).arg(code).arg(desc);
}

void onConnected(void *)
{
    ON_DEBUG
}

void onDisconnected(void *)
{
    ON_DEBUG
}

void onLoginSuccess(void *)
{
    TimTool::Instance().GetSelfProfile();
    emit TimTool::Instance().LoginSuccess();
}

void onLoginError(int code, const char *desc, void *data)
{
    QString str = desc;
    emit TimTool::Instance().LoginError(code, str);
}

void onCommSuccess(void *data)
{

}

void onCommError(int code, const char *desc, void *data)
{
    ERROR_DEBUG
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
    ERROR_DEBUG
}

void onAddFriendSuccess(TIMFriendResultHandle *handles, uint32_t num, void *data)
{
    TimTool::Instance().GetFriendList();
}

void onAddFriendError(int code, const char *desc, void *data)
{
    ERROR_DEBUG
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
    ERROR_DEBUG
}

void onGetNewMessage(TIMMessageHandle *handles, uint32_t msg_num, void *data)
{
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

        TIMConversationHandle conv = CreateConversation();
        GetConversationFromMsg(conv, handle);

        QString msg;
        int cnt = GetElemCount(handle);
        for(int j = 0; j < cnt; ++j)
        {
            TIMMsgElemHandle elem = GetElem(handle, j);
            TIMElemType type = GetElemType(elem);
            switch (type) {
            case TIMElemType::kElemText:
            {
//                uint32_t len = GetContentLen(elem);
                uint32_t len = 100;
                qDebug() << QString("length: %1").arg(len);
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                int ret = GetContent(elem, buffer, &len);
                qDebug() << QString("ret = %1, len = %2").arg(ret).arg(len);
//                for(int i = 0;i < len + 1; ++i)
//                    qDebug() << int(buffer[i]);
                QString s = QString::fromUtf8(buffer, len + 1);
                msg += s;
            }
                break;
            default:
                break;
            }
        }

        emit TimTool::Instance().NewMsg(sid, snick, msgTime, msg);
        DestroyProfileHandle(profile);
        DestroyConversation(conv);
    }
}
