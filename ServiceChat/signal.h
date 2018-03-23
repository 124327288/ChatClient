#ifndef SIGNAL_H
#define SIGNAL_H
#include <QString>
#include <QObject>
class Signal : public QObject
{
    Q_OBJECT
private:
    Signal();
public:
    static Signal &Instance();
private slots:
    void RemPwdAndAutoLoginHandle(bool isRemPwd, bool isAutoLogin);
//    void SetRememberPasswordHandle(bool checked);
//    void SetAutoLoginHandle(bool checked);
signals:
    void LoginSuccess();
    void LoginError(int code, const QString &desc);

    void GetFriendListSuccess(QList<Linkman> friendList);
    void GetFriendListError(int code, const char *desc);

    void GetSelfNickname(QString nick);

    void NewMsg(QString id, QString nick, uint32_t time, QString msg);

    void GetSignature(QString sig);
    void SetSignatureSuccess();

    void NewConversation(const QString &id, QString &nick, time_t time);

//    void SetRememberPassword(bool checked);
//    void SetAutoLogin(bool checked);
    void RemPwdAndAutoLogin(bool isRemPwd, bool isAutoLogin);
};
#endif // SIGNAL_H
