#include "luatool.h"
#include "timtool.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
//using std::string;
using namespace std;
LuaTool &LuaTool::Instance()
{
    static LuaTool instance;
    return instance;
}

void LuaTool::Init()
{
    L = luaL_newstate();
    luaL_openlibs(L);
}

LuaTool::LuaTool()
{
    path = ".\\config\\";
    appCfgFile = "app_cfg.lua";
    userCfgFile = "user_cfg.lua";
}

//    QFile data("file.txt");
//    if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
//        QTextStream out(&data);
//        out << "The answer is " << 42;
//    }

#define getGlobal(isfunc, tofunc, var, varStr, typeStr) {       \
    lua_getglobal(L, varStr);                                   \
    if(isfunc(L, -1)) {                                         \
        var = tofunc(L, -1);                                    \
        lua_pop(L, 1);                                          \
    }else{                                                      \
        varTypeError(varStr, typeStr);                          \
    }                                                           \
}

#define getGoNumber(var, varStr) getGlobal(lua_isnumber, lua_tonumber, (var), (varStr), "number")
#define getGoString(var, varStr) getGlobal(lua_isstring, lua_tostring, (var), (varStr), "string")
#define getGoBoolean(var, varStr) getGlobal(lua_isboolean, lua_toboolean, (var), (varStr), "boolean")

void LuaTool::getUserConfig()
{
    string pf = path + userCfgFile;
    if(luaL_dofile(L, pf.data()))
    {
		QMessageBox::information(nullptr, "ERROR!", QString("cannot do file %1").arg(pf.data()));
        qDebug() << QString("cannot do file %1").arg(pf.data());
    }
    getGoBoolean(rememberPassword, "rememberPassword");
    getGoBoolean(autoLogin, "autoLogin");
}

void LuaTool::getAppConfig()
{
    string pf = path + appCfgFile;
    if(luaL_dofile(L, pf.data()))
    {
		QMessageBox::information(nullptr, "ERROR!", QString("cannot do file %1").arg(pf.data()));
        qDebug() << QString("cannot do file %1").arg(pf.data());
    }
    getGoString(serverAddress, "serverAddress");
    getGoNumber(port, "port");
}

void LuaTool::getConfigs()
{
    getAppConfig();
    getUserConfig();
}

int LuaTool::getPort() const
{
    return port;
}

void LuaTool::setPort(int value)
{
    port = value;
}

std::string LuaTool::getServerAddress() const
{
    return serverAddress;
}

void LuaTool::setServerAddress(const std::string &value)
{
    serverAddress = value;
}

bool LuaTool::getAutoLogin() const
{
    return autoLogin;
}

void LuaTool::setAutoLogin(bool value)
{
    autoLogin = value;
}

bool LuaTool::getRememberPassword() const
{
    return rememberPassword;
}

void LuaTool::setRememberPassword(bool value)
{
    rememberPassword = value;
}
