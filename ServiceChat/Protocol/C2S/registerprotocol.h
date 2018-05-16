#ifndef REGISTERPROTOCOL_H
#define REGISTERPROTOCOL_H
#include "c2sprotocol.h"

class RegisterProtocol : public C2SProtocol
{
public:
    RegisterProtocol():C2SProtocol(PROTOCOLTYPE::Register)
    {

    }
    // C2SProtocol interface
    QString username() const;
    void setUsername(const QString &username);

    QString password() const;
    void setPassword(const QString &password);

private:
    void Marshal() override;
private:
    QString m_username;
    QString m_password;
};

#endif // REGISTERPROTOCOL_H
