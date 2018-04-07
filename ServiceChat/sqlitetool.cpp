#include "sqlitetool.h"
#include <QMessageBox>
#include "stdafx.h"
#include "databasetool.h"
#include "Table/table.h"
#include <vector>
using namespace std;
QString SqliteTool::dbName = "sys.db";

DatabaseTool SqliteTool::Database()
{
    DatabaseTool instance;
    instance.Bind(dbName);
    return instance;
}

void SqliteTool::CreateAll(DatabaseTool &dbTool)
{
    if(!dbTool.IsBind())
    {
        qDebug() << __func__ << "dbTool.Bind(dbName) ERROR";
        return;
    }
    Id id;
    Account account;
    Sig sig;
    if(!dbTool.IsTableExist(id))
        dbTool.CreateTable(id);
    if(!dbTool.IsTableExist(account))
        dbTool.CreateTable(account);
    if(!dbTool.IsTableExist(sig))
        dbTool.CreateTable(sig);
}
