#ifndef USERPWD_H
#define USERPWD_H
#include "c2sprotocol.h"
class UserPwdProtocol : public C2SProtocol
{
public:
    UserPwdProtocol():C2SProtocol(PROTOCOLTYPE::USERPWD)
    {

    }

    // C2SProtocol interface
private:
    void Marshal() override;
public:
    QString getUsername() const;
    void setUsername(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

private:
    QString username;
    QString password;
};

#endif // USERPWD_H
