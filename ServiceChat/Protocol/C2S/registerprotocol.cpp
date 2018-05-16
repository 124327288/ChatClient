#include "registerprotocol.h"
void RegisterProtocol::Marshal()
{
    QByteArray uarr = username().toUtf8();
    byte ul = uarr.length();
    Add(ul);
    Add(username());
    QByteArray parr = password().toUtf8();
    byte pl = parr.length();
    Add(pl);
    Add(password());
}

QString RegisterProtocol::password() const
{
    return m_password;
}

void RegisterProtocol::setPassword(const QString &password)
{
    m_password = password;
}

QString RegisterProtocol::username() const
{
    return m_username;
}

void RegisterProtocol::setUsername(const QString &username)
{
    m_username = username;
}
