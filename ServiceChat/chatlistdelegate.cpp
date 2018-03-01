#include "chatlistdelegate.h"

ChatListDelegate::ChatListDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void ChatListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

QSize ChatListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize();
}
