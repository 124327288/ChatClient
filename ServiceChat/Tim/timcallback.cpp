#include "timcallback.h"
#include "timtool.h"
#include <QUuid>
#include <memory>
#include <fstream>
#include <QCoreApplication>
#include <QFileDialog>
#include <mainwindow.h>
#include "loginwindow.h"
void onConnected(void *data)
{
    DEBUG_FUNC;
}

void onDisconnected(void *data)
{
    DEBUG_FUNC;
}

void onLoginSuccess(void *data)
{
    emit TimTool::Instance().OnLoginSuccess(data);
}

void onLoginError(int code, const char *desc, void *data)
{
    emit TimTool::Instance().OnLoginError(code, desc, data);
}

//void onCommSuccess(void *data)
//{

//}

//void onCommError(int code, const char *desc, void *data)
//{
//    DEBUG_ERROR;
//}

void onGetFriendListSuccess(TIMFriendListElemHandle *handles, uint32_t num, void *data)
{
    QList<Linkman> friendList;
    for(uint32_t i = 0; i < num; ++i)
    {
        TIMFriendListElemHandle handle = handles[i];
        QString id;
        QString nick;
        GET_ELEMENT(GetID4FriendListElemHandle, handle, &id);
        GET_ELEMENT(GetNickName4FriendListElemHandle,handle, &nick);
        friendList += {id, nick};
    }
    emit TimTool::Instance().GetFriendListSuccess(friendList);
}

void onGetFriendListError(int code, const char *desc, void *data)
{
    DEBUG_ERROR;
}

void onAddFriendSuccess(TIMFriendResultHandle *handles, uint32_t num, void *data)
{
    TimTool::Instance().GetFriendList();
}

void onAddFriendError(int code, const char *desc, void *data)
{
    DEBUG_ERROR;
}

void onGetSelfProfileSuccess(TIMSelfProfileHandle *handles, uint32_t num, void *data)
{
//    char nick[MAXLENNICK];
//	uint32_t len = MAXLENNICK;
//    ON_INVOKE(GetNickName4SlefProfileHandle, *handles, nick, &len);
//    QString sNick = QString::fromUtf8(nick);
    QString nick;
    GET_ELEMENT(GetNickName4SlefProfileHandle, *handles, &nick);
    TimTool::Instance().setNick(nick);
    emit TimTool::Instance().GetSelfNickname(nick);
}

void onGetSelfProfileError(int code, const char *desc, void *data)
{
    DEBUG_ERROR;
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
        QString id;
        QString nick;
        GET_ELEMENT(GetID4ProfileHandle, profile, &id);
        if(id == TimTool::Instance().getId())
            continue;
        GET_ELEMENT(GetNickName4ProfileHandle, profile, &nick);
        DestroyProfileHandle(profile);
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
            switch (type)
            {
                case kElemText:
                    onGetText(static_cast<TIMMsgTextElemHandle>(elem), id, nick, msgTime);
                    break;
                case kElemImage:
                    onGetImage(static_cast<TIMMsgImageElemHandle>(elem), id, nick, msgTime);
                    break;
                case kElemFile:
                    onGetFile(static_cast<TIMMsgFileElemHandle>(elem), id, nick, msgTime);
                    break;
            }
            DestroyElem(elem);
        }
    }
}

void onSetNickNameSuccess(void *data)
{
    TimTool::Instance().GetSelfProfile();
}

void onSetNickNameError(int code, const char *desc, void *data)
{
    DEBUG_ERROR;
}

void onGetImageSuccess(void *data)
{
    ChatContent *p_cc = static_cast<ChatContent*>(data);
    if(p_cc)
    {
        emit TimTool::Instance().NewMsg(p_cc->id, p_cc->nick, p_cc->time, p_cc->text);
        delete p_cc;
    }
}

void onGetImageError(int code, const char *desc, void *data)
{
    DEBUG_ERROR;
    ChatContent *p_cc = static_cast<ChatContent*>(data);
    if(p_cc)
        delete p_cc;
}

void onGetText(TIMMsgTextElemHandle handle, const QString &id, const QString &nick, time_t time)
{
    QString msg;
    GetElement<MAXLENCONTENT>(GetContent, __func__, handle, &msg);
//    GET_ELEMENT(GetContent, handle, &msg);
    qDebug() << QString("receive content = %1").arg(msg);
    emit TimTool::Instance().NewMsg(id, nick, time, msg);
}

void onGetImage(TIMMsgImageElemHandle handle, const QString &id, const QString &nick, time_t time)
{
    DEBUG_FUNC;
    uint32_t len = MAXLENIMAGE;
    TIMImageHandle imgHandles[MAXLENIMAGE] = {0};
    ON_INVOKE(GetImages, handle, imgHandles, &len);
    for(uint32_t i = 0; i < len; ++i)
    {
        TIMImageHandle imgHandle = imgHandles[i];
        TIMImageType imgType = GetImageType(imgHandle);
        if(imgType == TIMImageType::kOriginalImage)
        {
            QString dir = GetPicCacheDirName();
            QString str_uuid = UuidToStringEx();
            QString path = QDir::currentPath() + dir + str_uuid;
            DEBUG_VAR(path);
            QByteArray path_byte = path.toUtf8();
            QString msg = QString(R"(<p><img src = "%1" /></p>)").arg(path);
            ChatContent *p_cc = new ChatContent{id, nick, time, msg};
            TIMCommCB cb;
            cb.OnSuccess = &onGetImageSuccess;
            cb.OnError = &onGetImageError;
            cb.data = p_cc;
            qDebug() << QString("url: %1").arg(str_uuid);
            ON_INVOKE(GetImageFile, imgHandle, path_byte.data(), &cb);
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
    QString fileName;
    GET_ELEMENT(GetFileElemFileName, handle, &fileName);
    DEBUG_VAR(fileName);
    TIMGetMsgDataCB cb;
    cb.OnSuccess = &onGetFileSuccess;
    cb.OnError = &onGetFileError;
    cb.data = new ChatContent{id, nick, time, fileName};
    GetFileFromFileElem(handle, &cb);
}

void onGetFileSuccess(const char *buf, uint32_t len, void *data)
{
    DEBUG_FUNC;
    ChatContent *pChatContent = static_cast<ChatContent*>(data);
    if(pChatContent)
    {
        QString cachePath = QDir::currentPath() + GetCacheDirName();
        QString path = cachePath + pChatContent->text;
        std::wstring fileName = path.toStdWString();
//        QFileDialog::

        std::fstream file(fileName.data(), std::fstream::out | std::fstream::binary);
        std::string str_buf(buf, len);
        file << str_buf;
        emit TimTool::Instance().GetFileSuccess(pChatContent->id, pChatContent->nick, pChatContent->time, pChatContent->text, path, cachePath);
//        QString ofStr = QObject::tr("Open File");
//        QString ofJs = QString("javascript:onTest()");
//        QString ocfStr = QObject::tr("Open Containing Folder");
//        QString ocfJs = QString("javascript:openFolder('%1')").arg(QDir::currentPath());
//        QString msg = QString(R"z(
//                              <p>%1<a href = "%2">%3</a><a href = "%4">%5</a></p>
//                              )z").arg(pChatContent->text).arg(ofJs).arg(ofStr).arg(ocfJs).arg(ocfStr);

//        emit TimTool::Instance().NewMsg(pChatContent->id, pChatContent->nick, pChatContent->time, msg);
        delete pChatContent;
    }
}

void onGetFileError(int code, const char *desc, void *data)
{
    DEBUG_ERROR;
}

void onSendMsgSuccess(void *data)
{

}

void onSendMsgError(int code, const char *desc, void *data)
{
    DEBUG_ERROR;
}

void onSendImageSuccess(void *data)
{

}

void onSendImageError(int code, const char *desc, void *data)
{
    DEBUG_ERROR;
}

void onSendFileSuccess(void *data)
{

}

void onSendFileError(int code, const char *desc, void *data)
{
    DEBUG_ERROR;
}

void onKickOffline(void *data)
{
    DEBUG_FUNC;
    emit TimTool::Instance().OnKickOffline(data);
}

void onLogoutSuccess(void *data)
{
    Restart();
}

void onLogoutError(int code, const char *desc, void *data)
{
    DEBUG_FUNC;
}
