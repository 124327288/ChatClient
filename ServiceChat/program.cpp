#include "program.h"

time_t GetTime()
{
    time_t t;
    std::time(&t);
    return t;
}
