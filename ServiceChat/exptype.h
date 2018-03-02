#ifndef EXPTYPE_H
#define EXPTYPE_H
#include "expdef.h"
#include <QString>
#include <QList>
#include <QMetaType>
enum PROTOCOLTYPE : byte
{
    USERPWD = 1,
    SIGNATURE = 2,
    LOGINRES
};
enum PROGRAMSTATE
{
    NOPAGE = 0,
    LOGINPAGE = 1,
    MAINPAGE,
    EXIT,
};
enum LOGINRESTYPE : byte
{
    SUCCESS = 0,
    NOUSER = 1,
    PWDERROR = 2,
};

struct Linkman
{
    QString id;
    QString nick;
    QString remark;
};
Q_DECLARE_METATYPE(Linkman)
Q_DECLARE_METATYPE(QList<Linkman>)
Q_DECLARE_METATYPE(uint32_t)

struct ChatTitle
{
    QString id;
    QString nick;
    QString data;
    QString time;
};

struct ChatContent
{
    ChatTitle title;
    QString text;
};
#endif // EXPTYPE_H
