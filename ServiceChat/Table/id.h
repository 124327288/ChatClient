#ifndef ID_H
#define ID_H

#include <QObject>
#include <algorithm>
class Id : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(time_t time READ time WRITE setTime)
    QString m_id;

    time_t m_time;

public:
    Id(QObject *parent = nullptr):QObject(parent){}
    Id(const Id &t)
    {
        m_id = t.m_id;
        m_time = t.m_time;
    }
    Id &operator =(const Id &t)
    {
        m_id = t.m_id;
        m_time = t.m_time;
//        std::swap(*this, Id(t));
        return *this;
    }
    QString id() const
    {
        return m_id;
    }
    time_t time() const
    {
        return m_time;
    }

public slots:
    void setId(QString id)
    {
        m_id = id;
    }
    void setTime(time_t time)
    {
        m_time = time;
    }
};

#endif // ID_H
