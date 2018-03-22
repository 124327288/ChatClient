#ifndef TABLE_H
#define TABLE_H
#include <QString>
namespace Table
{
    class Account
    {
        QString id;
        QString pwd;

    public:
        QString getId() const;
        void setId(const QString &value);
        QString getPwd() const;
        void setPwd(const QString &value);
    };

    class Sign
    {
        QString id;
        QString sig;

    public:
        QString getId() const;
        void setId(const QString &value);
        QString getSig() const;
        void setSig(const QString &value);
    };
}

#endif // TABLE_H
