#include "program.h"

Program &Program::Instance()
{
    static Program instance;
    return instance;
}

Program::Program()
{

}
