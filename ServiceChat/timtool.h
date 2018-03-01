#ifndef TIMTOOL_H
#define TIMTOOL_H
#include <tim_c.h>
#include <tim_friend_c.h>
#include <QString>
#include <string>
#include <QObject>
#include <QDebug>
#include "exptype.h"
class TimTool : public QObject
{
    Q_OBJECT
public:
    static TimTool &Instance();
    void Init();
    void SetConnCallBack();
    void Login(const QString &username, const QString &password);
    QString getSig() const;
    void setSig(const QString &value);
    void GetFriendList();
    void AddSingleFriend(QString id, QString nick = nullptr, QString remark = nullptr, QString addWord = nullptr, QString addSource = nullptr);
    void GetSelfProfile();
    void SetMessageCallback();
signals:
    void LoginSuccess();
    void LoginError(int code, const QString &desc);
    void GetFriendListSuccess(QList<Linkman> friendList);
    void GetFriendListError(int code, const char *desc);
    void GetSelfNickname(QString nick);
private:
    TimTool();
    int          sdk_app_id;
    std::string  str_app_id;
    std::string  account_type;
    std::string  private_key;
    QString      sig;
    QString      id;
    QString      pwd;
public:
//    QStringList friendList;
    QString getId() const;
    QString getPwd() const;
    void setId(const QString &value);
    void setPwd(const QString &value);
};

void onLoginSuccess(void*);
void onLoginError(int code, const char *desc, void *data);
#endif // TIMTOOL_H
