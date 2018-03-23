#ifndef SQLITETOOL_H
#define SQLITETOOL_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "table.h"
class SqliteTool
{
private:
    SqliteTool();
public:
    static SqliteTool &Instance();
    bool IsOpen() const;
    bool CreateConnect();

    bool IsIdTableExist();
    bool IsAccountTableExist();
    bool IsSignTableExist();

    bool CreateIdTable();
    bool CreateAccountTable();
    bool CreateSignTable();

    bool Insert2IdTable(const QString &id);
    bool Insert2AccountTable(const QString &id, const QString &pwd);
    bool Insert2SignTable(const QString &id, const QString &sig);

    bool SelectAll4IdTable(QStringList *idList);
    bool Select4IdTable(const QString &id);
    bool Select4AccountTable(const QString &id, QString *pwd);
    bool Select4SignTable(const QString &id, QString *sig);

    bool Update2AccountTable(const QString &id, const QString &pwd);
    bool Update2SignTable(const QString &id, const QString &sig);

    bool Delete4AccountTable(const QString &id, bool all = false);
    bool Delete4SignTable(const QString &id, bool all = false);

    QSqlDatabase getDataBase() const;
    void setDataBase(const QSqlDatabase &value);

private:
    QSqlDatabase dataBase;
    QString dbName;
    QString idTableName;
    QString accountTableName;
    QString signTableName;
};

#endif // SQLITETOOL_H
