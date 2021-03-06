﻿#include "friendlistmodel.h"
#include "Tim/timtool.h"
FriendListModel::FriendListModel(QObject *parent) :QAbstractListModel(parent)
{
    if(!TimTool::Instance().getFriendList().isEmpty())
        m_friendList = TimTool::Instance().getFriendList();
    connect(&TimTool::Instance(), &TimTool::GetFriendListSuccess, this, &FriendListModel::updateList);
}

int FriendListModel::rowCount(const QModelIndex & parent) const
{
    return m_friendList.count();
}

QVariant FriendListModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid())
        return QVariant();
	switch (role)
	{
//    case Qt::DisplayRole:
    case Role::RoleID:
        return m_friendList[index.row()].id;
    case Role::RoleNick:
        return m_friendList[index.row()].nick;
//    case Role::RoleRemark:
//        return linkmanList[index.row()].remark;
//    case Qt::BackgroundColorRole:
//        return QColor(0x5c, 0xac, 0xee);
	default:
        return QVariant();
    }
}

void FriendListModel::updateList(QList<Linkman> list)
{
    beginResetModel();
    m_friendList = list;
    endResetModel();
//    for(auto man : list)
//    {
//        qDebug() << QString("id : %1, nick : %2, remark : %3").arg(man.id).arg(man.nick).arg(man.remark);
//    }
}

QVariant FriendListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
        return QString::number(section);
    return QVariant();

}
