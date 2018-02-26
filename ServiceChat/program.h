#ifndef PROGRAM_H
#define PROGRAM_H
#include "exptype.h"

class Program
{
public:
    static Program &Instance();
    PROGRAMSTATE state;
private:
    Program();
};

#endif // PROGRAM_H
