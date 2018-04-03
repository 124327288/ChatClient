#ifndef TABLE_H
#define TABLE_H
#include <QString>
namespace Table
{
    class Table;
    QString GetTableName(int i);
    QString GetTableName(const Table &table);

    class Table
    {
    public:
        virtual int GetType() const = 0;
        virtual int ColoumsNum() const = 0;
        virtual int GetIth(QString name) const = 0;
        virtual QString GetColName(int i) const = 0;
    };

    class Id : public Table
    {
        QString id;
    public:
        QString getId() const;
        void setId(const QString &value);
        // Table interface
    public:
        int GetType() const override { return 0; }
        int ColoumsNum() const override { return 1; }
        int GetIth(QString name) const override;
        QString GetColName(int i) const override;
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
        // Table interface
    public:
        int GetType() const override { return 1; }
        int ColoumsNum() const override { return 2; }
        int GetIth(QString name) const override;
        QString GetColName(int i) const override;
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
        // Table interface
    public:
        int GetType() const override { return 2; }
        int ColoumsNum() const override { return 2; }
        int GetIth(QString name) const override;
        QString GetColName(int i) const override;
    };
}

#endif // TABLE_H
