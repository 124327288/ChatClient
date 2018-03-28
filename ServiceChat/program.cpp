#include "program.h"

time_t GetTime()
{
    time_t t;
    std::time(&t);
    return t;
}

bool QCopyFile(const QString &src, const QString &desDir, bool cover)
{
    if(src == desDir)
        return true;
    if(!QFile::exists(src))
        return false;
    return QFile::copy(src, desDir);
}
