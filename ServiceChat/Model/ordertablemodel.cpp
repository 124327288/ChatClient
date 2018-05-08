#include "ordertablemodel.h"

OrderTableModel::OrderTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_orderList({
                    {"00000123" , "PRADA 机器人拼皮休闲鞋", 1},
                    {"00000123" , "PRADA 机器人拼皮休闲鞋", 1},
                    {"00000123" , "PRADA 机器人拼皮休闲鞋", 1},
                  })
{

}

int OrderTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_orderList.size();
}

int OrderTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 3;
}

QVariant OrderTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
        case Qt::DisplayRole:
        {
            int r = index.row();
            int c = index.column();
            switch (c) {
            case 0:
                return m_orderList[r].id;
            case 1:
                return m_orderList[r].name;
            case 2:
                return m_orderList[r].cnt;
            }
        }
    }
    return QVariant();
}

QList<Order> OrderTableModel::orderList() const
{
    return m_orderList;
}

void OrderTableModel::setOrderList(const QList<Order> &orderList)
{
    m_orderList = orderList;
}

QVariant OrderTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QString header[] = {"ID", "名称", "数量"};
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return header[section];
    }
    return {};
}
