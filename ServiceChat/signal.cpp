#include "signal.h"
#include "luatool.h"
Signal &Signal::Instance()
{
    static Signal instance;
    return instance;
}

void Signal::RemPwdAndAutoLoginHandle(bool isRemPwd, bool isAutoLogin)
{
    if(isRemPwd == LuaTool::Instance().getRememberPassword() &&
            isAutoLogin == LuaTool::Instance().getAutoLogin())
        return;
//    LuaTool::Instance().set
}

void Signal::SetRememberPasswordHandle(bool checked)
{
    if(checked == LuaTool::Instance().getRememberPassword())
        return;

}

void Signal::SetAutoLoginHandle(bool checked)
{

}
