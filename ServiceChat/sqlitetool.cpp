#include "sqlitetool.h"
#include <QMessageBox>
#include "stdafx.h"
SqliteTool::SqliteTool():
    dbName("Chat.db"),
    idTableName("Id"),
    accountTableName("Account"),
    signTableName("Sign")
{


}

SqliteTool &SqliteTool::Instance()
{
    static SqliteTool instance;
    return instance;
}

void SqliteTool::Init()
{
    if(!IsIdTableExist())
        CreateIdTable();
    if(!IsAccountTableExist())
        CreateAccountTable();
    if(!IsSignTableExist())
        CreateSignTable();
}

bool SqliteTool::IsOpen() const
{
    if(!dataBase.isOpen())
        QMessageBox::critical(nullptr, QObject::tr("DataBase is not open"),
                      QObject::tr("DataBase is not open"));
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

void SqliteTool::ShowAllTableName()
{
    if(!IsOpen())
        return;
    QSqlQuery query;
    if(!query.prepare(QString("select name from sqlite_master")))
    {
        SQL_ERROR(query);
        return;
    }
    if(!query.exec())
    {
        SQL_ERROR(query);
        return;
    }
    while (query.next())
    {
        QString tableName = query.value(0).toString();
        DEBUG_VAR(tableName);
    }
}

bool SqliteTool::IsTableExist(const QString &tableName, const QString &funcName)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("SELECT * FROM sqlite_master WHERE name = ?")))
    {
        SqlError(query, funcName);
        return false;
    }
    query.addBindValue(tableName);
    if(!query.exec())
    {
        SqlError(query, funcName);
        return false;
    }
    if(query.next())
    {
        return true;
    }
    return false;
}

bool SqliteTool::IsIdTableExist()
{
    return IsTableExist(idTableName, __func__);
}

bool SqliteTool::IsAccountTableExist()
{
    return IsTableExist(accountTableName, __func__);
}

bool SqliteTool::IsSignTableExist()
{
    return IsTableExist(signTableName, __func__);
}

bool SqliteTool::CreateIdTable()
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("CREATE TABLE %1(id TEXT PRIMARY KEY)").arg(idTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::CreateAccountTable()
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("CREATE TABLE %1(id TEXT PRIMARY KEY,pwd TEXT)").arg(accountTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::CreateSignTable()
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("CREATE TABLE %1(id TEXT PRIMARY KEY,sig TEXT)").arg(signTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::Insert2IdTable(const QString &id)
{
    DEBUG_FUNC;
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("INSERT INTO %1 (id) VALUES (?)").arg(idTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    query.addBindValue(id);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::Insert2AccountTable(const QString &id, const QString &pwd)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("INSERT INTO %1 (id, pwd) VALUES (?, ?)").arg(accountTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    query.addBindValue(id);
    query.addBindValue(pwd);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::Insert2SignTable(const QString &id, const QString &sig)
{
    DEBUG_FUNC;
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("INSERT INTO %1 (id, sig) VALUES (?, ?)").arg(signTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    query.addBindValue(id);
    query.addBindValue(sig);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::SelectAll4IdTable(QStringList *idList)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("SELECT * FROM Id")))
    {
        SQL_ERROR(query);
        return false;
    }
//    query.addBindValue(idTableName);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    while (query.next())
    {
        QString _id = query.value("id").toString();
        DEBUG_VAR(_id);
        (*idList) << _id;
//        return true;
    }
    return false;
}

bool SqliteTool::Select4IdTable(const QString &id)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("SELECT id FROM %1 WHERE id = ?").arg(idTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    query.addBindValue(id);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    while (query.next())
    {
        QString _id = query.value("id").toString();
        DEBUG_VAR(_id);
        return true;
    }
    return false;
}

bool SqliteTool::Select4AccountTable(const QString &id, QString *pwd)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("SELECT id, pwd FROM %1 WHERE id = ?").arg(accountTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    query.addBindValue(id);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    while (query.next())
    {
        QString _id = query.value("id").toString();
        QString _pwd = query.value("pwd").toString();
        DEBUG_VAR(_id);
        DEBUG_VAR(_pwd);
        if(pwd)
            *pwd = _pwd;
        return true;
    }
    return false;
}

bool SqliteTool::Select4SignTable(const QString &id, QString *sig)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("SELECT id, sig FROM %1 WHERE id = ?").arg(signTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    query.addBindValue(id);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    while (query.next())
    {
        QString _id = query.value("id").toString();
        QString _sig = query.value("sig").toString();
        DEBUG_VAR(_id);
        DEBUG_VAR(_sig);
        if(sig)
            *sig = _sig;
        return true;
    }
    return false;
}

bool SqliteTool::Update2AccountTable(const QString &id, const QString &pwd)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("UPDATE %1 SET pwd = ? WHERE id = ?").arg(accountTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    query.addBindValue(pwd);
    query.addBindValue(id);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::Update2SignTable(const QString &id, const QString &sig)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("UPDATE %1 SET sig = ? WHERE id = ?").arg(signTableName)))
    {
        SQL_ERROR(query);
        return false;
    }
    query.addBindValue(sig);
    query.addBindValue(id);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::Delete4AccountTable(const QString &id, bool all)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(all)
    {
        if(!query.prepare(QString("DELETE FROM %1").arg(accountTableName)))
        {
            SQL_ERROR(query);
            return false;
        }
    }
    else
    {
        if(!query.prepare(QString("DELETE FROM %1 WHERE id = ?").arg(accountTableName)))
        {
            SQL_ERROR(query);
            return false;
        }
        query.addBindValue(id);
    }
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool SqliteTool::Delete4SignTable(const QString &id, bool all)
{
    if(!IsOpen())
        return false;
    QSqlQuery query;
    if(all)
    {
        if(!query.prepare(QString("DELETE FROM %1").arg(signTableName)))
        {
            SQL_ERROR(query);
            return false;
        }
    }
    else
    {
        if(!query.prepare(QString("DELETE FROM %1 WHERE id = ?").arg(signTableName)))
        {
            SQL_ERROR(query);
            return false;
        }
        query.addBindValue(id);
    }
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
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
