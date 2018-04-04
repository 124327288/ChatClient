#include "databasetool.h"
#include "stdafx.h"
#include <QSqlError>
DatabaseTool::DatabaseTool()
{

}

DatabaseTool &DatabaseTool::Instance()
{
    static DatabaseTool instance;
    return instance;
}

bool DatabaseTool::Bind(const QString &dbName, const char *type)
{
    db = QSqlDatabase::addDatabase(type);
    db.setDatabaseName(dbName);
    if(!db.open())
    {
        SQL_ERROR(db);
        return false;
    }
    return true;
}

bool DatabaseTool::IsBind()
{
    if(!db.isOpen())
        QMessageBox::critical(nullptr, QObject::tr("DataBase is not open"),
                      QObject::tr("DataBase is not open"));
    return db.isOpen();
}

void DatabaseTool::ShowAllTableName()
{
    if(!IsBind())
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

bool DatabaseTool::IsTableExist(const QString &tableName)
{
    if(!IsBind())
        return false;
    QSqlQuery query;
    if(!query.prepare(QString("SELECT * FROM sqlite_master WHERE name = ?")))
    {
        SQL_ERROR(query);
        return false;
    }
    query.addBindValue(tableName);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    if(query.next())
    {
        return true;
    }
    return false;
}

bool DatabaseTool::IsTableExist(const char *tableName)
{
    IsTableExist(QString(tableName));
}

bool DatabaseTool::Update(const QString &tableName, const QVector<ParamType> &setList, const QVector<ParamType> &whereList)
{
    if(!IsBind())
        return false;
    if(setList.count() <= 0)
        return false;
    QString s = QString("UPDATE %1 SET ");
    for(int i = 0; i < setList.count(); ++i)
    {
        QString name = QString(" %1 = ? ").arg(setList[i].name);
        QString tmp = i == 0 ? name : QString(" , ") + name;
        s += tmp;
    }
    if(whereList.count() > 0)
    {
        s += " WHERE ";
        for(int i = 0; i < whereList.count(); ++i)
        {
            QString name = QString(" %1 = ? ").arg(whereList[i].name);
            QString tmp = i == 0 ? name : QString(" AND ") + name;
            s += tmp;
        }
    }
    QSqlQuery query;
    if(!query.prepare(s))
    {
        SQL_ERROR(query);
        return false;
    }
    for(const ParamType &p : setList)
        query.addBindValue(p.value);
    for(const ParamType &p : whereList)
        query.addBindValue(p.value);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

bool DatabaseTool::Delete(const QString &tableName, const QVector<ParamType> &whereList)
{
    if(!IsBind())
        return false;
    QString s = QString("DELETE FROM %1");
    if(whereList.count() > 0)
    {
        s += " WHERE ";
        for(int i = 0; i < whereList.count(); ++i)
        {
            QString name = QString(" %1 = ? ").arg(whereList[i].name);
            QString tmp = i == 0 ? name : QString(" AND ") + name;
            s += tmp;
        }
    }
    QSqlQuery query;
    if(!query.prepare(s))
    {
        SQL_ERROR(query);
        return false;
    }
    for(const ParamType &p : whereList)
        query.addBindValue(p.value);
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}
