#ifndef PROGRAM_H
#define PROGRAM_H
#include "stdafx.h"

#include <QWidget>
#include <ctime>

//#ifdef _DEBUG
//#define DEBUG_FUNCNAME   qDebug() << __func__;
//#else
//#define DEBUG_FUNCNAME
//#endif // _DEBUG

time_t GetTime();

inline void varTypeError(const QString &variable, const QString &type)
{
	qDebug() << QString("%1 is not %2 type").arg(variable).arg(type);
}

template <typename T>
inline void onDebugVariable(const QString &name, const T &val)
{
	qDebug() << QString("%1: %2").arg(name).arg(val);
}

inline void onDebugFunc(QString name)
{
	qDebug() << name;
}

inline void onDebugError(QString name, int code, const char *desc)
{
	qDebug() << QString("[%1] code = %2, desc = %3").arg(name).arg(code).arg(desc);
}

inline int onGetElementReturn(const char *funcName, int ret)
{
	if (ret)
	{
		qDebug() << QString("On %1 Error! RetCode = %2").arg(funcName).arg(ret);
	}
	return ret;
}

template <class T>
inline T d(T &&val)
{
	return std::move(val);
}

inline QString d(const std::string &val)
{
	return QString::fromStdString(val);
}

#define DEBUG_VAR(x)			onDebugVariable(#x, x)
#define DEBUG_FUNC				onDebugFunc(__func__);
#define DEBUG_ERROR				onDebugError(__func__, code, desc);
#define ON_INVOKE(func, ...)	onGetElementReturn(#func, func(__VA_ARGS__))

#define ListenCallBack(prcClassName)    \
    prc = new prcClassName(bytes);      \
    prc->UnMarshal();                   \
    On##prcClassName(prc);

#define PrcDynamicCast(prcClassName)                            \
    prcClassName *castPrc = dynamic_cast<prcClassName*>(prc);   \
    if(castPrc == nullptr) return;

#endif // PROGRAM_H
