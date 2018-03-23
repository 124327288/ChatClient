#ifndef LUATOOL_H
#define LUATOOL_H
#include "stdafx.h"
#include <string>
#include <lua.hpp>
class LuaTool : public QObject
{
    Q_OBJECT
public:
    static LuaTool &Instance();
    void Init();

    bool isDirExist() const;
    bool genDir();

    bool isUserDirExist() const;
    bool genUserDir();

    bool isAppCfgFileExist() const;
    bool isUserCfgFileExist() const;

    void genAppCfgFile() const;
    void genUserCfgFile() const;

    void updateAppCfgFile() const;
    void updateUserCfgFile() const;

    void getAppConfig();
    void getUserConfig();

    void getConfigs();

    bool getRememberPassword() const;
    void setRememberPassword(bool value);

    bool getAutoLogin() const;
    void setAutoLogin(bool value);

    std::string getServerAddress() const;
    void setServerAddress(const std::string &value);

    int getPort() const;
    void setPort(int value);

    std::string getUserDir() const;
    void setUserDir(const std::string &value);

    std::string getLanguage() const;
    void setLanguage(const std::string &value);
private slots:
    void RemPwdAndAutoLoginHandle(bool isRemPwd, bool isAutoLogin);
private:
    LuaTool();
    lua_State *L;
    std::string dir;
    std::string userDir;
    std::string appCfgFile;
    std::string userCfgFile;

    bool rememberPassword;
    bool autoLogin;
    std::string language;

    std::string serverAddress;
    int port;
};

#endif // LUATOOL_H
