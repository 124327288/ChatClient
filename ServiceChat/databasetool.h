#ifndef DATABASETOOL_H
#define DATABASETOOL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "table.h"
#include "stdafx.h"

class DatabaseTool
{
public:
    DatabaseTool();
    bool Bind(const char *dbName, const char *type = "QSQLITE");
    bool IsBind();

    template<typename T>
    bool Select(T *table, const QVector<ParamType> &list);
private:
    QSqlDatabase db;
};

template<typename T>
bool DatabaseTool::Select(T *table, const QVector<ParamType> &list)
{
    const char *tableName = Table::GetTableName(*table);
    if(!IsBind())
        return false;
    QSqlQuery q;
    if(list.empty())
    {
        QString s = QString("SELECT * FROM %1").arg(tableName);
        if(!q.prepare(s))
        {
            SQL_ERROR(q);
            return false;
        }
        if(!q.exec())
        {
            SQL_ERROR(q);
            return false;
        }
        while(q.next())
        {
//            q.value(0)
//            table->set()
        }
    }
    QString s = QString("SELECT * FROM %1 WHERE ").arg(tableName);
    for(const ParamType &p : list)
    {
        QString tmp = QString("%1 = ?").arg(p.name);
    }
}

#endif // DATABASETOOL_H
