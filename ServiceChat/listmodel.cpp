#include "listmodel.h"
#include "timtool.h"
ListModel::ListModel(QObject *parent) :QAbstractListModel(parent), stringList(TimTool::Instance().friendList)
{
//    TimTool::Instance().GetFriendList();
//    stringList = ;
}

int ListModel::rowCount(const QModelIndex & parent) const
{
	return stringList.count();
}

QVariant ListModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		return stringList[index.row()];
	default:
		return QVariant();
	}
}
