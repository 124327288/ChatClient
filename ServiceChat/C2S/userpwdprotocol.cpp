#include "userpwdprotocol.h"
#include <QDebug>
void UserPwdProtocol::Marshal()
{

//    QByteArray arr;
//    arr += (byte)type;

    byte ul = username.length();
    Add(ul);
    Add(username);
    byte pl = password.length();
    Add(pl);
    Add(password);
//    arr += ul;

//    arr += username;

//
//    arr += pl;

//    arr += password;

//    socket->write(arr.trimmed());
}

QString UserPwdProtocol::getUsername() const
{
    return username;
}

void UserPwdProtocol::setUsername(const QString &value)
{
    username = value;
}

QString UserPwdProtocol::getPassword() const
{
    return password;
}

void UserPwdProtocol::setPassword(const QString &value)
{
    password = value;
}
