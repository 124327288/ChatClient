#ifndef ID_H
#define ID_H

#include <QObject>
class Id : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(time_t time READ time WRITE setTime)
    QString m_id;

    time_t m_time;

public:
    Id() = default;
    Id(const Id &obj)
    {
        m_id = obj.m_id;
        m_time = obj.m_time;
    }
    Id &operator =(const Id &obj)
    {
        std::swap(*this, Id(obj));
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
