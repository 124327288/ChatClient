#include "signal.h"
#include "luatool.h"
#include "sqlitetool.h"
#include "tim/timtool.h"
#include "Table/table.h"
#include "databasetool.h"
Signal::Signal()
{
    connect(this, &Signal::RemPwdAndAutoLogin, this, &Signal::RemPwdAndAutoLoginHandle);
}

Signal &Signal::Instance()
{
    static Signal instance;
    return instance;
}

void Signal::RemPwdAndAutoLoginHandle(bool isRemPwd, bool isAutoLogin)
{
    if(isAutoLogin)
        isRemPwd = true;
    QString id = TimTool::Instance().getId();
    QString pwd = TimTool::Instance().getPwd();
    QString sig = TimTool::Instance().getSig();
    if(isRemPwd)
    {
        DatabaseTool db;
        QVector<Account> res;
        Account account;
        account.setId(id);
        account.setPwd(pwd);

        if(db.Select(&res, {{"id", id}}))
        {
            db.Update(account, {{"pwd", pwd}}, {{"id", id}});
        }
        else
        {
            db.Insert(account);
        }
    }
    if(isAutoLogin)
    {
        DatabaseTool db;
        QVector<Sig> res;
        Sig _sig;
        _sig.setId(id);
        _sig.setSig(sig);
        if(db.Select(&res, {{"id", id}}))
        {
            db.Update(sig, {{"sig", sig}}, {{"id", id}});
        }
        else
        {
            db.Insert(sig);
        }
    }
}

//void Signal::SetRememberPasswordHandle(bool checked)
//{
//    if(checked == LuaTool::Instance().getRememberPassword())
//        return;
//    SqliteTool::Instance().
//}

//void Signal::SetAutoLoginHandle(bool checked)
//{

//}
