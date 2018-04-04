#ifndef SQLITETOOL_H
#define SQLITETOOL_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "databasetool.h"
class SqliteTool
{
private:
    SqliteTool();
public:
    static SqliteTool &Instance();
    void Init();
private:
    DatabaseTool dbTool;
    QString dbName;
    QString idTableName;
    QString accountTableName;
    QString signTableName;
};

#endif // SQLITETOOL_H
