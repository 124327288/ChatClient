#ifndef SQLITETOOL_H
#define SQLITETOOL_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class SqliteTool
{
private:
    SqliteTool();
public:
    static SqliteTool &Instance();
    bool IsOpen() const;
    bool CreateConnect();
    bool CreateAccountTable();

    bool Insert2Table(const QString &tableName);
    bool Insert2AccountTable(const QString &id, const QString &pwd, const QString &sig);
    bool Select4AccountTable(const QString &id);
    QSqlDatabase getDataBase() const;
    void setDataBase(const QSqlDatabase &value);

private:
    QSqlDatabase dataBase;
    QString dbName;
    QString accountTableName;
};

#endif // SQLITETOOL_H
