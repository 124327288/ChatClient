#ifndef EXPDEF_H
#define EXPDEF_H

using byte      =   unsigned __int8;
using ubyte     =   unsigned __int8;
using sbyte     =   __int8;

using int16     =   __int16;
using Int16     =   __int16;
using uint16    =   unsigned __int16;
using UInt16    =   unsigned __int16;

using int32     =   __int32;
using Int32     =   __int32;
using uint32    =   unsigned __int32;
using UInt32    =   unsigned __int32;

using int64     =   __int64;
using Int64     =   __int64;
using uint64    =   unsigned __int64;
using UInt64    =   unsigned __int64;

enum ChatRole
{
    ChatRoleID = Qt::UserRole + 1,
    ChatRoleNick,
    ChatRoleData,
    ChatRoleTime,
    ChatRoleContent
};
#endif // EXPDEF_H
