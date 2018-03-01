#include "listmodel.h"
#include "timtool.h"
ListModel::ListModel(QObject *parent) :QAbstractListModel(parent)
{
     connect(&TimTool::Instance(), &TimTool::GetFriendListSuccess, this, &ListModel::updateList);
}

int ListModel::rowCount(const QModelIndex & parent) const
{
    return linkmanList.count();
}

QVariant ListModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
        return linkmanList[index.row()].id;
	default:
		return QVariant();
    }
}

void ListModel::updateList(QList<Linkman> list)
{
    beginInsertRows(QModelIndex(), rowCount(), list.count());
    linkmanList = list;
    endInsertRows();
    for(auto man : list)
    {
        qDebug() << QString("id : %1, nick : %2, remark : %3").arg(man.id).arg(man.nick).arg(man.remark);
    }
}
