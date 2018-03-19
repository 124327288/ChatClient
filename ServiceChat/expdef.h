#ifndef EXPDEF_H
#define EXPDEF_H
#include "qnamespace.h"
#include <stdint.h>
using byte      =   unsigned char;
using ubyte     =   unsigned char;
using sbyte     =   char;

using int16     =   int16_t;
using Int16     =   int16_t;
using uint16    =   uint16_t;
using UInt16    =   uint16_t;

using int32     =   int32_t;
using Int32     =   int32_t;
using uint32    =   uint32_t;
using UInt32    =   uint32_t;

using int64     =   int64_t;
using Int64     =   int64_t;
using uint64    =   uint64_t;
using UInt64    =   uint64_t;

enum Role
{
    RoleID = Qt::UserRole + 1,
    RoleNick,
    RoleRemark,
    RoleData,
    RoleTime,
    RoleContent
};

constexpr int MAXLENID = 1024;
constexpr int MAXLENNICK = 1024;
constexpr int MAXLENCONTENT = 10240;
#endif // EXPDEF_H
