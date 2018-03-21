#include "timcallback.h"
#include "timtool.h"
#include <QUuid>
#include <memory>
#include <fstream>

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

        char id_arr[MAXLENID];
        char nick_arr[MAXLENNICK];
        uint32_t idLen = MAXLENID;
        uint32_t nickLen = MAXLENNICK;
        ON_INVOKE(GetID4ProfileHandle, profile, id_arr, &idLen);
        ON_INVOKE(GetNickName4ProfileHandle, profile, nick_arr, &nickLen);
        QString id = QString::fromUtf8(id_arr, idLen);
        QString nick = QString::fromUtf8(nick_arr, nickLen);
        //qDebug() << QString("%1(%2)").arg(sid).arg(snick);
        if(!TimTool::Instance().ContainInConvMap(id))
        {
            TIMConversationHandle conv = CreateConversation();
            ON_INVOKE(GetConversationFromMsg, conv, handle);
            TimTool::Instance().AddConvMap(id, conv);
        }

        int cnt = GetElemCount(handle);
        for(int j = 0; j < cnt; ++j)
        {
            TIMMsgElemHandle elem = GetElem(handle, j);
            TIMElemType type = GetElemType(elem);
            switch (type) {
            case TIMElemType::kElemText:
                onGetText(static_cast<TIMMsgTextElemHandle>(elem), id, nick, msgTime);
                break;
            case TIMElemType::kElemImage:
                onGetImage(static_cast<TIMMsgImageElemHandle>(elem), id, nick, msgTime);
                break;
            case TIMElemType::kElemFile:
                onGetFile(static_cast<TIMMsgFileElemHandle>(elem), id, nick, msgTime);
                break;
            default:
                break;
            }
        }
        DestroyProfileHandle(profile);
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

void onGetImageSuccess(void *data)
{
//    ChatContent *p_cc = (ChatContent*)data;
    ChatContent *p_cc = static_cast<ChatContent*>(data);
    if(p_cc)
    {
        emit TimTool::Instance().NewMsg(p_cc->id, p_cc->nick, p_cc->time, p_cc->text);
        delete p_cc;
    }
}

void onGetImageError(int code, const char *desc, void *data)
{
    DEBUG_ERROR
    ChatContent *p_cc = static_cast<ChatContent*>(data);
    if(p_cc)
        delete p_cc;
}

void onGetText(TIMMsgTextElemHandle handle, const QString &id, const QString &nick, time_t time)
{
    uint32_t len = MAXLENCONTENT;
    char buffer[MAXLENCONTENT] = {0};
    ON_INVOKE(GetContent, handle, buffer, &len);
    QString msg = QString::fromUtf8(buffer, len);
    qDebug() << QString("receive content = %1").arg(msg);
    emit TimTool::Instance().NewMsg(id, nick, time, msg);
}

void onGetImage(TIMMsgImageElemHandle handle, const QString &id, const QString &nick, time_t time)
{
    uint32_t len = MAXLENIMAGE;
    TIMImageHandle imgHandles[MAXLENIMAGE] = {0};
    ON_INVOKE(GetImages, handle, imgHandles, &len);
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
            ChatContent *p_cc = new ChatContent{id, nick, time, msg};
            TIMCommCB cb;
            cb.OnSuccess = &onGetImageSuccess;
            cb.OnError = &onGetImageError;
            cb.data = p_cc;
            qDebug() << QString("url: %1").arg(str_uuid);
            ON_INVOKE(GetImageFile, imgHandle, uuid_byte.data(), &cb);
            break;
        }
    }
}

//TIM_DECL void		SetFileElemFileName(TIMMsgFileElemHandle handle, const char* name, uint32_t name_len);
//TIM_DECL int		GetFileElemFileName(TIMMsgFileElemHandle handle, char* name, uint32_t* name_len);
//TIM_DECL void		SetFileElemFilePath(TIMMsgFileElemHandle handle, const char* path, uint32_t path_len);
//TIM_DECL void		SetFileElemData(TIMMsgFileElemHandle handle, const char* data, uint32_t data_len);
//TIM_DECL int		GetFileElemUUID(TIMMsgFileElemHandle handle, char* id, uint32_t* len);
//TIM_DECL void		GetFileFromFileElem(TIMMsgFileElemHandle handle, TIMGetMsgDataCB* cb);
//TIM_DECL void		GetFileFromByUUID(const char *uuid, const uint32_t business_id, TIMGetMsgDataCB* cb);

void onGetFile(TIMMsgFileElemHandle handle, const QString &id, const QString &nick, time_t time)
{
    char filename_arr[MAXLENFILENAME] = {0};
    uint32_t len = MAXLENFILENAME;
    ON_INVOKE(GetFileElemFileName, handle, filename_arr, &len);
    QString *fileName = new QString(QString::fromUtf8(filename_arr, len));
    DEBUG_VAR(*fileName);
    TIMGetMsgDataCB cb;
    cb.OnSuccess = &onGetFileSuccess;
    cb.OnError = &onGetFileError;
    cb.data = fileName;
    GetFileFromFileElem(handle, &cb);
}


void onGetFileSuccess(const char *buf, uint32_t len, void *data)
{
    QString *pFileName = static_cast<QString*>(data);
    if(pFileName)
    {
        std::string fileName = pFileName->toStdString();
        std::fstream file(fileName.data(), std::fstream::out | std::fstream::binary);
        std::string str_buf(buf, len);
        file << str_buf;
        delete pFileName;
    }

}

void onGetFileError(int code, const char *desc, void *data)
{
    DEBUG_ERROR
}

void onSendMsgSuccess(void *data)
{

}

void onSendMsgError(int code, const char *desc, void *data)
{
    DEBUG_ERROR
}

void onSendImageSuccess(void *data)
{

}

void onSendImageError(int code, const char *desc, void *data)
{
    DEBUG_ERROR
}

void onSendFileSuccess(void *data)
{

}

void onSendFileError(int code, const char *desc, void *data)
{
    DEBUG_ERROR
}
