#ifndef TIMTOOL_H
#define TIMTOOL_H
#include <tim_c.h>
#include <tim_friend_c.h>
#include <QString>
#include <string>
#include <QObject>
#include <QDebug>
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
    void AddSingleFriend(QString id, QString nick, QString remark = nullptr, QString addWord = nullptr, QString addSource = nullptr);
signals:
    void LoginSuccess();
    void LoginError(int code, const QString &desc);
private:
    TimTool();
    int          sdk_app_id;
    std::string  str_app_id;
    std::string  account_type;
    std::string  private_key;
    QString      sig;
public:
    QStringList friendList;
};

void onLoginSuccess(void*);
void onLoginError(int code, const char *desc, void *data);

void onGetFriendListSuccess(TIMFriendListElemHandle* handles, uint32_t num, void* data);
void onGetFriendListError(int code, const char* desc, void* data);
#endif // TIMTOOL_H
