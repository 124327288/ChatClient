#pragma once
#include <QAbstractListModel>
#include <QList>
class ListModel : public QAbstractListModel
{
	Q_OBJECT
public:
    ListModel(QObject *parent = nullptr);
	// ͨ�� QAbstractListModel �̳�
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
private:
	QStringList stringList;
};
