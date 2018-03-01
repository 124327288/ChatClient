#pragma once
#include <QAbstractListModel>
#include <QList>
#include "exptype.h"
class ListModel : public QAbstractListModel
{
	Q_OBJECT
public:
    ListModel(QObject *parent = nullptr);
	// ͨ�� QAbstractListModel �̳�
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
private slots:
    void updateList(QList<Linkman> list);
private:
    QList<Linkman> linkmanList;
//	QStringList stringList;
};
