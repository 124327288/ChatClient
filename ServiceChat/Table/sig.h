#ifndef SIG_H
#define SIG_H

#include <QObject>

class Sig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString sig READ sig WRITE setSig)
    QString m_id;

    QString m_sig;

public:

    QString id() const
    {
        return m_id;
    }
    QString sig() const
    {
        return m_sig;
    }

public slots:
    void setId(QString id)
    {
        m_id = id;
    }
    void setSig(QString sig)
    {
        m_sig = sig;
    }
};

#endif // SIG_H
