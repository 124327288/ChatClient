#ifndef PROGRAM_H
#define PROGRAM_H
#include "stdafx.h"

class Program
{
public:
    static Program &Instance();
    PROGRAMSTATE state;
private:
    Program();
};

#endif // PROGRAM_H
