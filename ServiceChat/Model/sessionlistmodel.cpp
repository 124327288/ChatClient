#include "sessionlistmodel.h"
#include <QtAlgorithms>
SessionListModel::SessionListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int SessionListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return sessionList.count();
}

QVariant SessionListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch (role)
    {
    case Role::RoleID:
        return sessionList[index.row()].id;
    case Role::RoleNick:
        return sessionList[index.row()].nick;
    case Role::RoleRemark:
        return sessionList[index.row()].remark;
    default:
        return QVariant();
    }
}

bool SessionListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool SessionListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
//    for(int i = 0;)
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

void SessionListModel::Sort()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    qSort(sessionList.begin(), sessionList.end(), [](const Session &a, const Session &b){
        return a.time > b.time;
    });
    endInsertRows();
}
