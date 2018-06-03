#ifndef TIMTOOL_H
#define TIMTOOL_H
#include "stdafx.h"
#include "tim_header.h"
#include <string>
#include "chatwindow.h"
class TimTool : public QObject
{
    Q_OBJECT
private:
    TimTool();

public:
    static TimTool &Instance();
    void Init();
    void SetConnCallBack();
    void SetMessageCallback();
    void SetKickOfflineCallBack();
    void GetLocalMessage();
    void Logout();
private:
    bool TimPathExist() const;
    void MakeTimPath();
    void _Login();
public:
    void Login(const QString &username, const QString &password);

    void GetFriendList();
    void AddSingleFriend(QString id, QString nick = nullptr, QString remark = nullptr, QString addWord = nullptr, QString addSource = nullptr);

    void GetSelfProfile();

    void SetNickName(const QString &nick);

    void SendMsg(const QString &id, const QVector<TimMsg> &msgList);
    void SendMsg(QString id, QString text);
    void SendImage(const QString &id, const QString &imgPath);
    void SendFile(const QString &id, const QString &filePath);

    int AddChatWindowMap(QString id, ChatWindow *window);
    void UpdateChatWindowMap(QString id, ChatWindow *window);
    int RemoveChatWindowMap(QString id);
    ChatWindow *GetChatWindow(QString id);
    bool ContainInChatWindowMap(QString id);

    int AddConvMap(QString id, TIMConversationHandle handle);
    void UpdateConvMap(QString id, TIMConversationHandle handle);
    int RemoveConvMap(QString id);
    TIMConversationHandle GetConvHandle(QString id);
    bool ContainInConvMap(QString id);

    QVector<ChatContentEX>  GetContentEX(QString id);
    void                    ClearContentEX();
signals:
    void OnConnected(void *data);
    void OnDisconnected(void *data);

    void OnKickOffline(void *data);

    void OnLoginSuccess(void *data);
    void OnLoginError(int code, const char *desc, void *data);

    void OnLogoutSuccess(void *data);
    void OnLogoutError(int code, const char *desc, void *data);

    void OnGetFriendListSuccess(TIMFriendListElemHandle *handles, uint32_t num, void *data);
    void OnGetFriendListError(int code, const char *desc, void *data);

    void OnAddFriendSuccess(TIMFriendResultHandle* handles, uint32_t num, void* data);
    void OnAddFriendError(int code, const char* desc, void* data);

    void OnGetSelfProfileSuccess(TIMSelfProfileHandle* handles, uint32_t num, void* data);
    void OnGetSelfProfileError(int code, const char* desc, void* data);

    void GetFriendListSuccess(QList<Linkman> friendList);
    void GetFriendListError(int code, const char *desc);
    void GetSelfNickname(QString nick);
    void NewMsg(QString id, QString nick, time_t time, QString msg);
    void GetFileSuccess(const QString &id, const QString &nick, time_t time, const QString &fileName, const QString &filePath, const QString &folderPath);
    void GetSignature(QString sig);
    void SetSignatureSuccess();
    void NewConversation(const QString &id, QString &nick, time_t time);

private slots:
    void NewMsgHandler(QString id, QString nick, uint32_t time, QString msg);
    void OnKickOfflineHandle(void* data);
    void GetFileSuccessHandle(const QString &id, const QString &nick, time_t time, const QString &fileName, const QString &filePath, const QString &folderPath);
	void GetFriendListSuccessHandle(const QList<Linkman> &friendList);
private:
    int          sdk_app_id;
    std::string  str_app_id;
    std::string  account_type;
    std::string  private_key;
    std::string  timPath;
    QString      sig;
    QString      id;
    QString      nick;
    QString      pwd;

    QMap<QString, ChatWindow*>              chatWindowMap;
    QMap<QString, TIMConversationHandle>    convMap;
    QMap<QString, QVector<ChatContentEX>>   contentMap;
    QList<Linkman>                          friendList;
public:
    ~TimTool();
public:
    QString getSig() const;
    QString getId() const;
    QString getPwd() const;
    QString getNick() const;
    void setNick(const QString &value);
    QMap<QString, ChatWindow *> getChatWindowMap() const;
    void setChatWindowMap(const QMap<QString, ChatWindow *> &value);
    QMap<QString, TIMConversationHandle> getConvMap() const;
    void setConvMap(const QMap<QString, TIMConversationHandle> &value);
    QMap<QString, QVector<ChatContentEX> > getContentMap() const;
    void setContentMap(const QMap<QString, QVector<ChatContentEX> > &value);
    QList<Linkman> getFriendList() const;
    void setFriendList(const QList<Linkman> &value);
};
#define TimInstance (TimTool::Instance())
#endif // TIMTOOL_H
