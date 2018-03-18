#ifndef PROGRAM_H
#define PROGRAM_H
#include "stdafx.h"

#include <QWidget>
#include <ctime>
//class Program : public QWidget
//{
//    Q_OBJECT
//public:
//    static Program *Instance();
//    static QWidget *Widget();
//    PROGRAMSTATE state;
//public:
//    Program() = default;
//    static void setProgram(Program *value);

//private:
//    static Program *program;
//};
#ifdef _DEBUG
#define DEBUG_FUNCNAME   qDebug() << __func__;
#else
#define DEBUG_FUNCNAME
#endif // _DEBUG

time_t GetTime();
#endif // PROGRAM_H
