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

QRect GetRectFrom2Point(const QPoint &p1, const QPoint &p2)
{
    auto px = std::minmax(p1.x(), p2.x());
    auto py = std::minmax(p1.y(), p2.y());
    return QRect(QPoint(px.first, py.first), QPoint(px.second, py.second));
}
