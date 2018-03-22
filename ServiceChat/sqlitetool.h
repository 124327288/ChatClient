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
    bool CreateConnect();
private:

    QString dbName;

};

#endif // SQLITETOOL_H
