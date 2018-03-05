#include "program.h"

//Program *Program::program = nullptr;

//Program *Program::Instance()
//{
//    return program;
//}

//QWidget *Program::Widget()
//{
//    return static_cast<QWidget*>(program);
//}

//void Program::setProgram(Program *value)
//{
//    program = value;
//}

time_t GetTime()
{
    static time_t t;
    std::time(&t);
    return t;
}
