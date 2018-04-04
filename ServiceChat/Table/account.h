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
