#include "tim.h"
#include "stdafx.h"
QString Tim::GetID4FriendListElemHandle(TIMFriendListElemHandle handle)
{
    QString id;
    GET_ELEMENT(::GetID4FriendListElemHandle, handle, id);
    return id;
}

QString Tim::GetNickName4FriendListElemHandle(TIMFriendListElemHandle handle)
{
    QString nick;
    GET_ELEMENT(::GetNickName4FriendListElemHandle, handle, nick);
    return nick;
//    return GetElement4Handle<MAXLENNICK>(::GetNickName4FriendListElemHandle, handle);
}

QString Tim::GetRemark4FriendListElemHandle(TIMFriendListElemHandle handle)
{
    QString remark;
    GET_ELEMENT(::GetRemark4FriendListElemHandle, handle, remark);
    return remark;
//    return GetElement4Handle<MAXLENREMARK>(::GetNickName4FriendListElemHandle, handle);
}
