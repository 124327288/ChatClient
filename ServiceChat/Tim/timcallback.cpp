#include "timcallback.h"
#include "timtool.h"
#include <QUuid>
#include <memory>

void onConnected(void *)
{
    DEBUG_FUNC
}

void onDisconnected(void *)
{
    DEBUG_FUNC
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
    DEBUG_ERROR
}

void onGetFriendListSuccess(TIMFriendListElemHandle *handles, uint32_t num, void *data)
{
    QList<Linkman> friendList;
    for(uint32_t i = 0; i < num; ++i)
    {
        TIMFriendListElemHandle handle = handles[i];
        char id[MAXLENID];
        char nickName[MAXLENNICK];
//        char remark[16];
        uint32_t idLen = MAXLENID;
        uint32_t nickLen = MAXLENNICK;
//        uint32_t remarkLen;
        ON_INVOKE(GetID4FriendListElemHandle, handle, id, &idLen);
        ON_INVOKE(GetNickName4FriendListElemHandle, handle, nickName, &nickLen);
//        ON_INVOKE(GetRemark4FriendListElemHandle, handle, remark, &remarkLen);
        QString sId = QString::fromUtf8(id, idLen);
        QString sNick = QString::fromUtf8(nickName, nickLen);
//        QString sRemark = QString::fromUtf8(remark, remarkLen);
        friendList += {sId, sNick};
    }
    emit TimTool::Instance().GetFriendListSuccess(friendList);
}

void onGetFriendListError(int code, const char *desc, void *data)
{
    DEBUG_ERROR
}

void onAddFriendSuccess(TIMFriendResultHandle *handles, uint32_t num, void *data)
{
    TimTool::Instance().GetFriendList();
}

void onAddFriendError(int code, const char *desc, void *data)
{
    DEBUG_ERROR
}

void onGetSelfProfileSuccess(TIMSelfProfileHandle *handles, uint32_t num, void *data)
{
    char nick[MAXLENNICK];
	uint32_t len = MAXLENNICK;
    ON_INVOKE(GetNickName4SlefProfileHandle, *handles, nick, &len);
    QString sNick = QString::fromUtf8(nick);
    TimTool::Instance().setNick(sNick);
    emit TimTool::Instance().GetSelfNickname(sNick);
}

void onGetSelfProfileError(int code, const char *desc, void *data)
{
    DEBUG_ERROR
}

void onGetNewMessage(TIMMessageHandle *handles, uint32_t msg_num, void *data)
{
	qDebug() << QString("%1 ;Msg Num : %2").arg(__func__).arg(msg_num);
    for(uint32_t i = 0; i < msg_num; ++i)
    {
        TIMMessageHandle handle = handles[i];
        uint32_t msgTime = GetMsgTime(handle);
        TIMProfileHandle profile = CreateProfileHandle();
        ON_INVOKE(GetSenderProfile, handle, profile);

        char id[MAXLENID];
        char nick[MAXLENNICK];
        uint32_t idLen = MAXLENID;
        uint32_t nickLen = MAXLENNICK;
        ON_INVOKE(GetID4ProfileHandle, profile, id, &idLen);
        ON_INVOKE(GetNickName4ProfileHandle, profile, nick, &nickLen);
        QString sid = QString::fromUtf8(id, idLen);
        QString snick = QString::fromUtf8(nick, nickLen);
        //qDebug() << QString("%1(%2)").arg(sid).arg(snick);

        TIMConversationHandle conv = CreateConversation();
        ON_INVOKE(GetConversationFromMsg, conv, handle);
        TimTool::Instance().AddConvMap(sid, conv);
        int cnt = GetElemCount(handle);
        for(int j = 0; j < cnt; ++j)
        {
            TIMMsgElemHandle elem = GetElem(handle, j);
            TIMElemType type = GetElemType(elem);
            switch (type) {
            case TIMElemType::kElemText:
            {
                uint32_t len = MAXLENCONTENT;
                char buffer[MAXLENCONTENT] = {0};
                ON_INVOKE(GetContent, elem, buffer, &len);
                QString msg = QString::fromUtf8(buffer, len);
                qDebug() << QString("content = %1").arg(msg);
                emit TimTool::Instance().NewMsg(sid, snick, msgTime, msg);
                break;
            }
            case TIMElemType::kElemImage:
            {
                uint32_t len = MAXLENIMAGE;
                TIMImageHandle imgHandles[MAXLENIMAGE] = {0};
                ON_INVOKE(GetImages, elem, imgHandles, &len);
                for(uint32_t i = 0; i < len; ++i)
                {
                    TIMImageHandle imgHandle = imgHandles[i];
                    TIMImageType imgType = GetImageType(imgHandle);
                    if(imgType == TIMImageType::kOriginalImage)
                    {
                        QUuid uuid = QUuid::createUuid();
                        QString str_uuid = uuid.toString();
                        QByteArray uuid_byte = str_uuid.toUtf8();
                        QString msg = QString(R"(<img src = "%1" />)").arg(str_uuid);
                        ChatContent *p_cc = new ChatContent{sid, snick, msgTime, msg};
                        TIMCommCB cb;
                        cb.OnSuccess = &onGetImageFileSuccess;
                        cb.OnError = &onGetImageFileError;
                        cb.data = p_cc;
                        qDebug() << QString("url: %1").arg(str_uuid);
                        ON_INVOKE(GetImageFile, imgHandle, uuid_byte.data(), &cb);
                        break;
                    }
                }
                break;
            }
            default:
                break;
            }
        }
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
    DEBUG_ERROR
}

void onGetImageFileSuccess(void *data)
{
//    ChatContent *p_cc = (ChatContent*)data;
    ChatContent *p_cc = static_cast<ChatContent*>(data);
    if(p_cc)
    {
        emit TimTool::Instance().NewMsg(p_cc->id, p_cc->nick, p_cc->time, p_cc->text);
        delete p_cc;
    }
}

void onGetImageFileError(int code, const char *desc, void *data)
{
    DEBUG_ERROR
    delete data;
}
