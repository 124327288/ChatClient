#include "linkmanlistmodel.h"
#include "timtool.h"
LinkmanListModel::LinkmanListModel(QObject *parent) :QAbstractListModel(parent)
{
     connect(&TimTool::Instance(), &TimTool::GetFriendListSuccess, this, &LinkmanListModel::updateList);
}

int LinkmanListModel::rowCount(const QModelIndex & parent) const
{
    return linkmanList.count();
}

QVariant LinkmanListModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
        return linkmanList[index.row()].id;
	default:
		return QVariant();
    }
}

void LinkmanListModel::updateList(QList<Linkman> list)
{
    beginInsertRows(QModelIndex(), rowCount(), list.count());
    linkmanList = list;
    endInsertRows();
    for(auto man : list)
    {
        qDebug() << QString("id : %1, nick : %2, remark : %3").arg(man.id).arg(man.nick).arg(man.remark);
    }
}

QVariant LinkmanListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QAbstractListModel::headerData(section, orientation, role);
    return tr("Linkmanhe");

}
