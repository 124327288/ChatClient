#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>

class Account : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString pwd READ pwd WRITE setPwd)
    QString m_id;

    QString m_pwd;

public:
    Account() = default;
    Account(const Account &obj)
    {
        m_id = obj.m_id;
        m_pwd = obj.m_pwd;
    }
    Account &operator =(const Account &obj)
    {
        m_id = obj.m_id;
        m_pwd = obj.m_pwd;
        return *this;
    }

    QString id() const
    {
        return m_id;
    }
    QString pwd() const
    {
        return m_pwd;
    }

public slots:
    void setId(QString id)
    {
        m_id = id;
    }
    void setPwd(QString pwd)
    {
        m_pwd = pwd;
    }
};

#endif // ACCOUNT_H
