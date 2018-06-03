#include "ordertablemodel.h"
#include <random>
using namespace std;
OrderTableModel::OrderTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_orderList(OrderList())
{
    default_random_engine e;
    uniform_int_distribution<unsigned> u(1,10);
    for(Order_S &o : m_orderList)
    {
        if(o.filenum == 0)
        {
            unsigned num = u(e);
            float total = num * 0.1f;
            o.filenum = num;
            o.totalamt = total;
        }
    }
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
    return 4;
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
                return m_orderList[r].oid;
            case 1:
                return m_orderList[r].servid;
            case 2:
                return m_orderList[r].filenum;
            case 3:
                return m_orderList[r].totalamt;
            }
        }
    }
    return QVariant();
}

QList<Order_S> &OrderTableModel::orderList() const
{
    return m_orderList;
}

void OrderTableModel::setOrderList(const QList<Order_S> &orderList)
{
    m_orderList = orderList;
}

QVariant OrderTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QString header[] = {"订单ID", "服务ID", "打印数量", "总价"};
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return header[section];
    }
    return {};
}
