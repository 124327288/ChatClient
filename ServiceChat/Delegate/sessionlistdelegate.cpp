#include "sessionlistdelegate.h"
#include "stdafx.h"
SessionListDelegate::SessionListDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void SessionListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = QString("%1(%2)").arg(index.data(Role::RoleID).toString()).arg(index.data(Role::RoleNick).toString());
    painter->drawText(option.rect, text);
    QStyledItemDelegate::paint(painter, option, index);
}

QSize SessionListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 20);
}
