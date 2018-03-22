#include "sqlitetool.h"
#include <QMessageBox>
#include "stdafx.h"
SqliteTool::SqliteTool():dbName("Chat.db"), accountTableName("Account")
{

}

SqliteTool &SqliteTool::Instance()
{
    static SqliteTool instance;
    return instance;
}

bool SqliteTool::IsOpen() const
{
    return dataBase.isOpen();
}

bool SqliteTool::CreateConnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(!db.open())
    {
        SQL_ERROR(db);
        return false;
    }
    dataBase = db;
    return true;
}

bool SqliteTool::CreateAccountTable()
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.exec(QString(R"(
                           CREATE TABLE %1(
                           id VARCHAR PRIMARY KEY,
                           pwd VARCHAR,
                           sig VARCHAR)
                           )").arg(accountTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::Insert2Table(const QString &tableName)
{
    if(!IsOpen())
    {

    }
    return true;
}

bool SqliteTool::Insert2AccountTable(const QString &id, const QString &pwd, const QString &sig)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (id, pwd, sig) VALUES (?, ?, ?)").arg(accountTableName));
    query.addBindValue(id);
    query.addBindValue(pwd);
    query.addBindValue(sig);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::Select4AccountTable(const QString &id)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    query.exec(QString("SELECT id, pwd, sig FROM %1 WHERE id = %2").arg(accountTableName).arg(id));
    while (query.next())
    {
        QString id = query.value("id").toString();
        QString pwd = query.value("pwd").toString();
        QString sig = query.value("sig").toString();
        DEBUG_VAR(id);
        DEBUG_VAR(pwd);
        DEBUG_VAR(sig);
    }
    return true;

}

QSqlDatabase SqliteTool::getDataBase() const
{
    return dataBase;
}

void SqliteTool::setDataBase(const QSqlDatabase &value)
{
    dataBase = value;
}
