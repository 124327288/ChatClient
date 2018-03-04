#ifndef TIMTOOL_H
#define TIMTOOL_H
#include "stdafx.h"
#include "tim.h"
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

private:
    bool TimPathExist() const;
    void MakeTimPath();
    void _Login();
public:
    void Login(const QString &username, const QString &password);

    void GetFriendList();
    void AddSingleFriend(QString id, QString nick = nullptr, QString remark = nullptr, QString addWord = nullptr, QString addSource = nullptr);

    void GetSelfProfile();

    void SendMsg(QString id, QString text);

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
signals:
    void LoginSuccess();
    void LoginError(int code, const QString &desc);
    void GetFriendListSuccess(QList<Linkman> friendList);
    void GetFriendListError(int code, const char *desc);
    void GetSelfNickname(QString nick);
    void NewMsg(QString id, QString nick, uint32_t time, QString msg);
    void GetSignature(QString sig);
    void SetSignatureSuccess();
private slots:
    void NewMsgHandler(QString id, QString nick, uint32_t time, QString msg);
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

    QMap<QString, ChatWindow*>            chatWindowMap;
    QMap<QString, TIMConversationHandle>  convMap;

public:
    QString getSig() const;
    QString getId() const;
    QString getPwd() const;
    QString getNick() const;
};
#endif // TIMTOOL_H
