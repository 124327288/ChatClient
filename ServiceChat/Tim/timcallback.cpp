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
        char id[MAXLENID];
        char nickName[MAXLENNICK];
        char remark[16];
        uint32_t idLen, nickLen, remarkLen;
        GetID4FriendListElemHandle(handle, id, &idLen);
        GetNickName4FriendListElemHandle(handle, nickName, &nickLen);
        GetRemark4FriendListElemHandle(handle, remark, &remarkLen);
        QString sId = QString::fromUtf8(id, idLen);
        QString sNick = QString::fromUtf8(nickName, nickLen);
        QString sRemark = QString::fromUtf8(remark, remarkLen);
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

//    qDebug() << "GetSelfProfile num: " << num;
    char nick[16];
    uint32_t len;
    GetNickName4SlefProfileHandle(*handles, nick, &len);
    QString sNick = QString::fromUtf8(nick);
    TimTool::Instance().setNick(sNick);
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
        int ret = -1;
        TIMProfileHandle profile = CreateProfileHandle();
        ret = GetSenderProfile(handle, profile);
        onGetElementReturn("GetSenderProfile", ret);

        char id[MAXLENID];
        char nick[MAXLENNICK];
        uint32_t idLen;
        uint32_t nickLen;
        ret = GetID4ProfileHandle(profile, id, &idLen);
        onGetElementReturn("GetID4ProfileHandle", ret);
        ret = GetNickName4ProfileHandle(profile, nick, &nickLen);
        onGetElementReturn("GetNickName4ProfileHandle", ret);
        QString sid = QString::fromUtf8(id, idLen);
        QString snick = QString::fromUtf8(nick, nickLen);
        qDebug() << QString("%1(%2)").arg(sid).arg(snick);

        TIMConversationHandle conv = CreateConversation();
        GetConversationFromMsg(conv, handle);
//        GetConversationPeer
        TimTool::Instance().AddConvMap(sid, conv);

        QString msg;
        int cnt = GetElemCount(handle);
        for(int j = 0; j < cnt; ++j)
        {
            TIMMsgElemHandle elem = GetElem(handle, j);
            TIMElemType type = GetElemType(elem);
            switch (type) {
            case TIMElemType::kElemText:
            {
                uint32_t len = MAXLENCONTENT;
                char buffer[MAXLENCONTENT];
                memset(buffer, 0, sizeof(buffer));
                int ret = GetContent(elem, buffer, &len);
                QString s = QString::fromUtf8(buffer, len + 1);
                qDebug() << QString("ret = %1, content = %2").arg(ret).arg(s);
                msg += s;
                break;
            }
            default:
                break;
            }
        }

        emit TimTool::Instance().NewMsg(sid, snick, msgTime, msg);
        DestroyProfileHandle(profile);
//        DestroyConversation(conv);
    }
}

void onSetNickNameSuccess(void *data)
{
    TimTool::Instance().GetSelfProfile();
}

void onSetNickNameError(int code, const char *desc, void *data)
{
    ERROR_DEBUG
}

void onGetElementReturn(const char *funcName, int ret)
{
    if(ret)
    {
        qDebug() << QString("On %1 Error! Return = %2").arg(funcName).arg(ret);
    }
}
