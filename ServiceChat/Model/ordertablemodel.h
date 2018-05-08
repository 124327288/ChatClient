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

    QList<Order> orderList() const;
    void setOrderList(const QList<Order> &orderList);

private:
    QList<Order> m_orderList;

    // QAbstractItemModel interface
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

#endif // ORDERTABLEMODEL_H
