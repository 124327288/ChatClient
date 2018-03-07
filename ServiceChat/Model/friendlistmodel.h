#ifndef FRIENDLISTMODEL_H
#define FRIENDLISTMODEL_H
#include "stdafx.h"
#include <QAbstractListModel>
class FriendListModel : public QAbstractListModel
{
	Q_OBJECT
public:
    FriendListModel(QObject *parent = nullptr);
    // QAbstractListModel
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
private slots:
    void updateList(QList<Linkman> list);
private:
    QList<Linkman> linkmanList;

    // QAbstractItemModel interface
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
#endif // FRIENDLISTMODEL_H
