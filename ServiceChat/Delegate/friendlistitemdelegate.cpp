#include "friendlistitemdelegate.h"
#include "stdafx.h"
FriendListItemDelegate::FriendListItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void FriendListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = QString("%1(%2)").arg(index.data(Role::RoleID).toString()).arg(index.data(Role::RoleNick).toString());
    painter->drawText(option.rect, text);
    QStyledItemDelegate::paint(painter, option, index);
}

QSize FriendListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 20);
}
