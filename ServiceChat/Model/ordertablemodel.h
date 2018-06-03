#ifndef ORDERTABLEMODEL_H
#define ORDERTABLEMODEL_H

#include "stdafx.h"
#include <QAbstractTableModel>

class OrderTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit OrderTableModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<Order_S> &m_orderList;

    // QAbstractItemModel interface
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QList<Order_S> &orderList() const;
    void setOrderList(const QList<Order_S> &orderList);
};

#endif // ORDERTABLEMODEL_H
