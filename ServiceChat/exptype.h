#ifndef EXPTYPE_H
#define EXPTYPE_H
#include "expdef.h"
#include <QMetaType>
#include <QString>
enum PROTOCOLTYPE : byte
{
    USERPWD = 1,
    SIGNATURE = 2,
    LOGINRES,
    Register
};

enum PROGRAMSTATE
{
    NOPAGE = 0,
    LOGINPAGE = 1,
    MAINPAGE,
    EXIT,
};

enum class LOGINRESTYPE : byte
{
    SUCCESS = 0,
    NOUSER = 1,
    PWDERROR = 2,
    FAIL = 3,
};

struct Linkman
{
    QString id;
    QString nick;
//    QString remark;
};

struct Session
{
    QString id;
    QString nick;
//    QString remark;
    time_t  time;
};

struct ChatTitle
{
    QString id;
    QString nick;
    time_t time;
};

struct ChatContent
{
    QString id;
    QString nick;
    time_t time;
    QString text;
};

struct ChatContentEX
{
    time_t time;
    QString text;
};

struct ChatContentIM
{
    bool isMine;
    time_t time;
    QString text;
};

struct ChatContentWithId
{
    QString from;
    QString to;
    time_t time;
    QString text;
};

struct AppCfgStruct
{
    std::string serverAddress;
    int port;
};

struct UserCfgStruct
{
    bool rememberPassword;
    bool autoLogin;
};

struct TimMsg
{
//    TimMsg(int t, QString m):type(t), msg(m) {}
    int type;
    QString msg;
};

struct ParamType
{
    const char *name;
    QVariant value;
};

struct Order
{
    QString id;
    QString name;
    int cnt;
};
Q_DECLARE_METATYPE(Order)
Q_DECLARE_METATYPE(Linkman)
Q_DECLARE_METATYPE(QList<Linkman>)
Q_DECLARE_METATYPE(uint32_t)
Q_DECLARE_METATYPE(time_t)

#endif // EXPTYPE_H
