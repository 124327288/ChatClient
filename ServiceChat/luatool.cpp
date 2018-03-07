#include "luatool.h"
#include "Tim/timtool.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QDir>
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
    if(!isDirExist())
        genDir();
    if(!isAppCfgFileExist())
        genAppCfgFile();
    if(!isUserCfgFileExist())
        genUserCfgFile();
}

bool LuaTool::isDirExist() const
{
    QDir _dir(dir.data());
    return _dir.exists();
}

bool LuaTool::genDir()
{
    QDir _dir;
    return _dir.mkdir(dir.data());
}

bool LuaTool::isAppCfgFileExist() const
{
    auto _path = dir + appCfgFile;
    QFile file(_path.data());
    return file.exists();
}

bool LuaTool::isUserCfgFileExist() const
{
    auto _path = dir + userCfgFile;
    QFile file(_path.data());
    return file.exists();
}

void LuaTool::genAppCfgFile() const
{
    qDebug() << "genAppCfgFile";
    auto _path = dir + appCfgFile;
    QFile file(_path.data());
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
//        serverAddress = "127.0.0.1"
//        port = 2333
        os << QString("%1 = \"%2\"\n").arg("serverAddress").arg("127.0.0.1");
        os << QString("%1 = %2\n").arg("port").arg(2333);
    }
}

void LuaTool::genUserCfgFile() const
{
    qDebug() << "genUserCfgFile";
    auto _path = dir + userCfgFile;
    QFile file(_path.data());
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
//        rememberPassword = false
//        autoLogin = false
        os << QString("%1 = %2\n").arg("rememberPassword").arg("false");
        os << QString("%1 = %2\n").arg("autoLogin").arg("false");
    }
}

LuaTool::LuaTool()
{
    dir = ".\\config\\";
    appCfgFile = "app_cfg.lua";
    userCfgFile = "user_cfg.lua";
}

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
    string pf = dir + userCfgFile;
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
    string pf = dir + appCfgFile;
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
