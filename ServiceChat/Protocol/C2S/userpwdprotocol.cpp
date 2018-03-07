#include "userpwdprotocol.h"
void UserPwdProtocol::Marshal()
{

//    QByteArray arr;
//    arr += (byte)type;
    QByteArray uarr = username.toUtf8();
    byte ul = uarr.length();
    Add(ul);
    Add(username);
    QByteArray parr = password.toUtf8();
    byte pl = parr.length();
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
