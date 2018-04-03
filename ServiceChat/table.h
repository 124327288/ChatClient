#ifndef TABLE_H
#define TABLE_H
#include <QMap>
#include <QString>
#include <QStringList>
namespace Table
{
    class Table;
    QString GetTableName(int i);
    QString GetTableName(const Table &table);
    class Table
    {
    public:
        virtual int Type() const = 0;
        virtual QStringList ColNameList() const = 0;
        int ColCount() const { return ColNameList().size(); }
        QString GetColName(int i) const { return ColNameList().at(i); }
        QMap<QString, int> NameToIdxMap() const;
        int GetIth(QString name) const { return NameToIdxMap().value(name); }
    };

    class Id : public Table
    {
        QString id;
    public:
        QString getId() const;
        void setId(const QString &value);
    };

    class Account : public Table
    {
        QString id;
        QString pwd;
    public:
        QString getId() const;
        void setId(const QString &value);
        QString getPwd() const;
        void setPwd(const QString &value);
    };

    class Sign : public Table
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
