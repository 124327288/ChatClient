#include "tim.h"
#include "stdafx.h"
QString Tim::GetID4FriendListElemHandle(TIMFriendListElemHandle handle)
{
    return GetElement4Handle<MAXLENID>(::GetID4FriendListElemHandle, handle);
}

QString Tim::GetNickName4FriendListElemHandle(TIMFriendListElemHandle handle)
{
    return GetElement4Handle<MAXLENNICK>(::GetNickName4FriendListElemHandle, handle);
}

QString Tim::GetRemark4FriendListElemHandle(TIMFriendListElemHandle handle)
{
    return GetElement4Handle<MAXLENNICK>(::GetNickName4FriendListElemHandle, handle);
}
