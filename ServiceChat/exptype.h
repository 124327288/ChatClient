#ifndef EXPTYPE_H
#define EXPTYPE_H
#include "expdef.h"
#include <QMetaType>
#include <QString>
enum PROTOCOLTYPE : byte
{
    USERPWD = 1,
    SIGNATURE = 2,
    LOGINRES = 3,
    REGISTER = 4,
    REGISTER_RET = 5,
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

enum class REGISTERRETTYPE : byte
{
    SUCCESS = 0,
    EXIST = 1,
    REGISTERERROR = 2,
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
    bool        rememberPassword;
    bool        autoLogin;
    std::string sendMsg;
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

//struct Order
//{
//    QString id;
//    QString name;
//    int cnt;
//};

struct Order_S
{
    QString oid;
    QString servid;
    int filenum;
    double totalamt;
};

using Order = Order_S;

Q_DECLARE_METATYPE(Order)
Q_DECLARE_METATYPE(Linkman)
Q_DECLARE_METATYPE(QList<Linkman>)
Q_DECLARE_METATYPE(uint32_t)
Q_DECLARE_METATYPE(time_t)

#endif // EXPTYPE_H
