﻿#ifndef LUATOOL_H
#define LUATOOL_H
#include "stdafx.h"
#include <string>
#include <lua.hpp>
class LuaTool : public QObject
{
    Q_OBJECT
private:
    LuaTool();
public:
    static LuaTool &Instance();
    void Init();
    ~LuaTool();
    bool isDirExist() const;
    bool genDir();

    static bool isUserDirExist(const std::string &_userDir);
    static bool genUserDir(const std::string &_userDir);

    std::string makeUserDirString(const QString &_id);
    std::string makeUserDirString(const std::string &_id);
    bool isUserDirExist() const;
    bool genUserDir();

    bool isAppCfgFileExist() const;
    bool isUserCfgFileExist() const;
    bool isUserCfgFileExist(const std::string &_userDir) const;
    bool isOrderCfgFileExist() const;

    void genAppCfgFile() const;
    void genUserCfgFile() const;
    void genOrderCfgFile() const;

    void updateAppCfgFile() const;
    void updateUserCfgFile() const;

    void getAppConfig();
    void getUserConfig(const std::string &_userDir, UserCfgStruct *pUserCfg);
    void getUserConfig();
    void getOrderConfig();

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
    QList<Order> getOrderList() const;
    void setOrderList(const QList<Order> &orderList);

    std::string getSendMsg() const;
    void setSendMsg(const std::string &value);

private slots:
    void LoginSuccessHandle();
    void RemPwdAndAutoLoginHandle(bool isRemPwd, bool isAutoLogin);
private:
    lua_State *L;
    std::string dir;
    std::string userDir;
    std::string appCfgFile;
    std::string userCfgFile;
    std::string orderCfgFile;
    QList<Order> m_orderList;

    bool rememberPassword;
    bool autoLogin;
    std::string sendMsg;
    std::string language;

    std::string serverAddress;
    int port;

    bool isGetLoginedUserCfg = false;

};
#define LuaInstance (LuaTool::Instance())
#endif // LUATOOL_H
