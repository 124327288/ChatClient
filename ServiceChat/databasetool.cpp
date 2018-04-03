#include "databasetool.h"
#include "stdafx.h"
#include <QSqlError>
DatabaseTool::DatabaseTool()
{

}

bool DatabaseTool::Bind(const char *dbName, const char *type)
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
