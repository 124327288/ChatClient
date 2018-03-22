#ifndef TIM_H
#define TIM_H
//tim_c.h	管理，负责基本的SDK操作	初始化登录、注销、创建会话等
//tim_conv_c.h	会话，负责会话相关操作	如发送消息，获取会话消息缓存，获取未读计数等
//tim_msg_c.h	消息	包括文本、图片等不同类型消息
//tim_group_c.h	群管理器	负责创建群，增删成员，以及修改群资料等
//tim_friend_c.h	资料关系链管理	负责获取、修改好友资料和关系链信息
#include <tim_c.h>
#include <tim_conv_c.h>
#include <tim_msg_c.h>
#include <tim_group_c.h>
#include <tim_friend_c.h>

class Tim
{
public:
    /* static */
    static QString GetID4FriendListElemHandle(TIMFriendListElemHandle handle);
    static QString GetNickName4FriendListElemHandle(TIMFriendListElemHandle handle);
    static QString GetRemark4FriendListElemHandle(TIMFriendListElemHandle handle);
};
#endif // TIM_H
