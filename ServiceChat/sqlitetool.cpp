#include "sqlitetool.h"
#include <QMessageBox>
#include "stdafx.h"
#include "databasetool.h"
#include "Table/table.h"
#include <vector>
using namespace std;
SqliteTool::SqliteTool():
    dbName("Chat.db"),
    idTableName(Id().metaObject()->className()),
    accountTableName(Account().metaObject()->className()),
    signTableName(Sig().metaObject()->className())
{


}

SqliteTool &SqliteTool::Instance()
{
    static SqliteTool instance;
    return instance;
}

void SqliteTool::Init()
{
    if(!dbTool.Bind(dbName))
    {
        qDebug() << "dbTool.Bind(dbName) ERROR";
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
