#include "ordertableitemdelegate.h"

OrderTableItemDelegate::OrderTableItemDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}

void OrderTableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

QSize OrderTableItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return {};
}
