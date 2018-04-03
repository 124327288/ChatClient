#include "luatool.h"
#include "Tim/timtool.h"
#include "signal.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QDir>
//using std::string;
using namespace std;

LuaTool::LuaTool()
{
    dir = ".\\config\\";
    appCfgFile = "app_cfg.lua";
    userCfgFile = "user_cfg.lua";
    connect(&TimTool::Instance(), &TimTool::OnLoginSuccess,this, &LuaTool::LoginSuccessHandle);
    connect(&Signal::Instance(), &Signal::RemPwdAndAutoLogin, this, &LuaTool::RemPwdAndAutoLoginHandle);
}

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
//    if(!isUserCfgFileExist())
    //        genUserCfgFile();
}

LuaTool::~LuaTool()
{
    if(L)
        lua_close(L);
    L = nullptr;
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

bool LuaTool::isUserDirExist(const string &_userDir)
{
    QDir _dir(_userDir.data());
    return _dir.exists();
}

bool LuaTool::genUserDir(const string &_userDir)
{
    QDir _dir;
    return _dir.mkdir(_userDir.data());
}

std::string LuaTool::makeUserDirString(const QString &_id)
{
    return dir + _id.toStdString() + '\\';
}

string LuaTool::makeUserDirString(const string &_id)
{
    return dir + _id + '\\';
}

bool LuaTool::isUserDirExist() const
{
    QDir _dir(userDir.data());
    return _dir.exists();
}

bool LuaTool::genUserDir()
{
    QDir _dir;
    return _dir.mkdir(userDir.data());
}

bool LuaTool::isAppCfgFileExist() const
{
    if(!isDirExist())
        return false;
    auto _path = dir + appCfgFile;
    QFile file(_path.data());
    return file.exists();
}

bool LuaTool::isUserCfgFileExist() const
{
    if(!isUserDirExist())
        return false;
    auto _path = userDir + userCfgFile;
    QFile file(_path.data());
    return file.exists();
}

bool LuaTool::isUserCfgFileExist(const string &_userDir) const
{
    if(!isUserDirExist(_userDir))
        return false;
    auto _path = _userDir + userCfgFile;
    QFile file(_path.data());
    return file.exists();
}

void LuaTool::genAppCfgFile() const
{
    DEBUG_FUNC;
    auto _path = dir + appCfgFile;
    QFile file(_path.data());
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        os << QString("%1 = \"%2\"\n").arg("serverAddress").arg("127.0.0.1");
        os << QString("%1 = %2\n").arg("port").arg(2333);
        os << QString("%1 = \"%2\"\n").arg("language").arg("cn");
    }
}

void LuaTool::genUserCfgFile() const
{
    DEBUG_FUNC;
    auto _path = userDir + userCfgFile;
    QFile file(_path.data());
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        os << QString("%1 = %2\n").arg("rememberPassword").arg("false");
        os << QString("%1 = %2\n").arg("autoLogin").arg("false");
//        os << QString("%1 = %2\n").arg("language").arg(R"("cn")");
    }
}

void LuaTool::updateAppCfgFile() const
{
    DEBUG_FUNC;
    auto _path = dir + appCfgFile;
    QFile file(_path.data());
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        os << QString("%1 = \"%2\"\n").arg("serverAddress").arg(serverAddress.data());
        os << QString("%1 = %2\n").arg("port").arg(port);
        os << QString("%1 = \"%2\"\n").arg("language").arg(language.data());
    }
}

void LuaTool::updateUserCfgFile() const
{
    DEBUG_FUNC;
    auto _path = userDir + userCfgFile;
    QFile file(_path.data());
    if(file.open(QFile::WriteOnly))
    {
        QTextStream os(&file);
        os << QString("%1 = %2\n").arg("rememberPassword").arg(bts(rememberPassword));
        os << QString("%1 = %2\n").arg("autoLogin").arg(bts(autoLogin));
    }
}

std::string LuaTool::getLanguage() const
{
    return language;
}

void LuaTool::setLanguage(const std::string &value)
{
    language = value;
}

void LuaTool::LoginSuccessHandle()
{
    QByteArray id_bytes = TimTool::Instance().getId().toUtf8();
    string id = id_bytes.data();
    setUserDir(makeUserDirString(id));
    if(!isUserDirExist())
    {
        genUserDir();
    }
    if(!isUserCfgFileExist())
    {
        genUserCfgFile();
    }
    getUserConfig();
}

void LuaTool::RemPwdAndAutoLoginHandle(bool isRemPwd, bool isAutoLogin)
{
    while(!isGetLoginedUserCfg);
    if(isRemPwd == getRememberPassword() && isAutoLogin == getAutoLogin())
        return;
    setRememberPassword(isRemPwd);
    setAutoLogin(isAutoLogin);
    updateUserCfgFile();
}

std::string LuaTool::getUserDir() const
{
    return userDir;
}

void LuaTool::setUserDir(const std::string &value)
{
    userDir = value;
}

#define getGlobal(isfunc, tofunc, var, varStr, typeStr) {       \
    lua_getglobal(L, varStr);                                   \
    if(isfunc(L, -1)) {                                         \
        var = tofunc(L, -1);                                    \
        lua_pop(L, 1);                                          \
    }else{                                                      \
        varTypeError(varStr, typeStr);                          \
        return;                                                 \
    }                                                           \
}

#define getGoNumber(var, varStr) getGlobal(lua_isnumber, lua_tonumber, (var), (varStr), "number")
#define getGoString(var, varStr) getGlobal(lua_isstring, lua_tostring, (var), (varStr), "string")
#define getGoBoolean(var, varStr) getGlobal(lua_isboolean, lua_toboolean, (var), (varStr), "boolean")

void LuaTool::getUserConfig()
{
    string pf = userDir + userCfgFile;
    if(luaL_dofile(L, pf.data()))
    {
//		QMessageBox::information(nullptr, "ERROR!", QString("cannot do file %1").arg(pf.data()));
        qDebug() << QString("cannot do file %1").arg(pf.data());
        return;
    }
    getGoBoolean(rememberPassword, "rememberPassword");
    getGoBoolean(autoLogin, "autoLogin");
    isGetLoginedUserCfg = true;
}

void LuaTool::getUserConfig(const string &_userDir, UserCfgStruct *pUserCfg)
{
    if(!pUserCfg)
        return;
    string pf = _userDir + userCfgFile;
    if(luaL_dofile(L, pf.data()))
    {
        QMessageBox::information(nullptr, "ERROR!", QString("cannot do file %1").arg(pf.data()));
        qDebug() << QString("cannot do file %1").arg(pf.data());
        return;
    }
    getGoBoolean(pUserCfg->rememberPassword, "rememberPassword");
    getGoBoolean(pUserCfg->autoLogin, "autoLogin");
}


void LuaTool::getAppConfig()
{
    string pf = dir + appCfgFile;
    if(luaL_dofile(L, pf.data()))
    {
		QMessageBox::information(nullptr, "ERROR!", QString("cannot do file %1").arg(pf.data()));
        qDebug() << QString("cannot do file %1").arg(pf.data());
        return;
    }
    getGoString(serverAddress, "serverAddress");
    getGoNumber(port, "port");
    getGoString(language, "language");
}

void LuaTool::getConfigs()
{
    getAppConfig();
//    getUserConfig();
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
