#include "friendlistdelegate.h"
#include "stdafx.h"
FriendListDelegate::FriendListDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void FriendListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = QString("%1(%2)").arg(index.data(Role::RoleID).toString()).arg(index.data(Role::RoleRemark).toString());
    painter->drawText(option.rect, text);
    QStyledItemDelegate::paint(painter, option, index);
}

QSize FriendListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 20);
}
