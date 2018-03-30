#include "sessionlistitemdelegate.h"
#include "stdafx.h"
SessionLisItemtDelegate::SessionLisItemtDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void SessionLisItemtDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = QString("%1(%2)").arg(index.data(Role::RoleID).toString()).arg(index.data(Role::RoleNick).toString());
    painter->drawText(option.rect, text);
    QStyledItemDelegate::paint(painter, option, index);
}

QSize SessionLisItemtDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 20);
}
