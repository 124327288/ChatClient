#ifndef LUATOOL_H
#define LUATOOL_H
#include <string>
#include <lua.hpp>
#include <QString>
#include <QDebug>
class LuaTool
{
public:
    static LuaTool &Instance();
    void Init();

    bool isDirExist() const;
    bool genDir();

    bool isAppCfgFileExist() const;
    bool isUserCfgFileExist() const;

    void genAppCfgFile() const;
    void genUserCfgFile() const;

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

private:
    LuaTool();
    lua_State *L;
    std::string dir;
    std::string appCfgFile;
    std::string userCfgFile;

    bool rememberPassword;
    bool autoLogin;

    std::string serverAddress;
    int port;
};

inline void varTypeError(const QString &variable, const QString &type)
{
    qDebug() << QString("%1 is not %2 type").arg(variable).arg(type);
}

template <class T>
inline T d(T val)
{
    return val;
}

inline QString d(const std::string &val)
{
    return val.data();
}

#endif // LUATOOL_H
