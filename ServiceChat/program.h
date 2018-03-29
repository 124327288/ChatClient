#ifndef PROGRAM_H
#define PROGRAM_H
#include "stdafx.h"

#include <QWidget>
#include <ctime>
#include <functional>
#include <QMessageBox>

//#ifdef _DEBUG
//#define DEBUG_FUNCNAME   qDebug() << __func__;
//#else
//#define DEBUG_FUNCNAME
//#endif // _DEBUG

time_t GetTime();

template <class T>
inline T d(const T &val)
{
    return val;
}

inline QString d(const std::string &val)
{
    return QString::fromStdString(val);
}

inline QString d(const std::wstring &val)
{
    return QString::fromStdWString(val);
}

inline const char *bts(bool b)
{
    return b ? "true" : "false";
}

inline void varTypeError(const QString &variable, const QString &type)
{
	qDebug() << QString("%1 is not %2 type").arg(variable).arg(type);
//    QMessageBox::critical(nullptr, QObject::tr("%1 Error").arg(variable), QString("%1 is not %2 type").arg(variable).arg(type));
}

template <typename T>
inline void onDebugVariable(const QString &name, const T &val)
{
    qDebug() << QString("%1: %2").arg(name).arg(d(val));
}

inline void onDebugFunc(QString name)
{
	qDebug() << name;
}

inline void onDebugError(QString name, int code, const char *desc)
{
    qDebug() << QString("[%1] code : %2, desc : %3").arg(name).arg(code).arg(desc);
//    QMessageBox::critical(nullptr, QObject::tr("%1 Error").arg(name), QString("code : %1, desc : %2").arg(code).arg(desc));
}

inline int onGetElementReturn(const char *funcName, int ret)
{
	if (ret)
	{
		qDebug() << QString("On %1 Error! RetCode = %2").arg(funcName).arg(ret);
//        QMessageBox::critical(nullptr, QObject::tr("%1 Error").arg(funcName), QString("%1 Error! RetCode = %2").arg(funcName).arg(ret));
	}
	return ret;
}

#define VAR_NAME(x)             (#x)
#define DEBUG_VAR(x)			(onDebugVariable(#x, x))
#define DEBUG_FUNC				(onDebugFunc(__func__))
#define DEBUG_ERROR				(onDebugError(__func__, code, desc))
#define ON_INVOKE(func, ...)	(onGetElementReturn(#func, func(__VA_ARGS__)))

#define GET_ELEMENT(func, handle, str)        {         \
    char buf[MAXLENBUFFER];                             \
    uint32_t len = MAXLENBUFFER;                        \
    int ret = ON_INVOKE(func, handle, buf, &len);       \
    if(!ret)                                            \
    {                                                   \
        str = QString::fromUtf8(buf, len);              \
    }                                                   \
}
//using GetElement4HandleType = int(*)(void*, char*, uint32_t*);

//template <uint32_t MaxLen = MAXLENBUFFER>
//QString GetElement4Handle(GetElement4HandleType func, void *handle)
//{
//    char buf[MaxLen];
//    uint32_t len = MaxLen;
//	int ret = ON_INVOKE(func, handle, buf, &len);
//	if (!ret)
//	{
//		return QString::fromUtf8(buf, len);
//	}
//	return nullptr;
//}

template <class T>
inline void SqlError(const T &sql, const QString &funcName)
{
    QMessageBox::critical(nullptr, QObject::tr("%1 Error").arg(funcName),
                      sql.lastError().text());
}

#define SQL_ERROR(sql)                                                      \
    QMessageBox::critical(nullptr, QObject::tr("%1 Error").arg(__func__),   \
                      sql.lastError().text())

//#define SQL_ERROR_EX(sql, funcName)                                                     \
//    QMessageBox::critical(nullptr, QObject::tr("%1 Error").arg(funcName),   \
//                      sql.lastError().text())

#define ListenCallBack(prcClassName)    \
    prc = new prcClassName(bytes);      \
    prc->UnMarshal();                   \
    On##prcClassName(prc);

#define PrcDynamicCast(prcClassName)                            \
    prcClassName *castPrc = dynamic_cast<prcClassName*>(prc);   \
    if(castPrc == nullptr) return;

inline QString GetCacheDirName()
{
    return "/Cache/";
}

inline QString GetPicCacheDirName()
{
    return "/TimPath/PicCache/";
}

inline bool IsDirExist(const QString &dir)
{
    return QDir(dir).exists();
}

inline bool GenDir(const QString &dir)
{
    return QDir().mkdir(dir);
}

inline bool GenCacheDir()
{
    if(!IsDirExist(GetCacheDirName()))
        return GenDir(GetCacheDirName());
}

template <typename T, typename T2>
inline T Range(T2 v, const T &left, const T &right)
{
    if(v < left)
        v = left;
    else if (v > right)
        v = right;
    return v;
}

template <typename T>
inline T Range(T v, const T &left, const T &right)
{
    if(v < left)
        v = left;
    else if (v > right)
        v = right;
    return v;
}


bool QCopyFile(const QString &src, const QString &des, bool cover = true);
QRect GetRectFrom2Point(const QPoint &p1, const QPoint &p2);
QRect InterSection2Rect(const QRect &r1, const QRect &r2);

#endif // PROGRAM_H
