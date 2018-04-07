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
    static DatabaseTool Database();
    static void CreateAll(DatabaseTool &dbTool);
private:
    static QString dbName;
};

#endif // SQLITETOOL_H
