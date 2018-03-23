#include "signal.h"
#include "luatool.h"
#include "sqlitetool.h"
#include "tim/timtool.h"
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
        if(SqliteTool::Instance().Select4AccountTable(id))
        {
            SqliteTool::Instance().Update2AccountTable(id, pwd);
        }
        else
        {
            SqliteTool::Instance().Insert2AccountTable(id, pwd);
        }
    }
    if(isAutoLogin)
    {
        if(SqliteTool::Instance().Select4SignTable(id))
        {
            SqliteTool::Instance().Update2SignTable(id, sig);
        }
        else
        {
            SqliteTool::Instance().Insert2SignTable(id, sig);
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
