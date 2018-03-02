#include "chatlistdelegate.h"
#include "exptype.h"
#include <QPainter>

ChatListDelegate::ChatListDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void ChatListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString title;
    QString id = index.data(ChatRoleID).toString();
    QString nick = index.data(ChatRoleNick).toString();
    QString data = index.data(ChatRoleData).toString();
    QString time = index.data(ChatRoleTime).toString();
    QString text = index.data(ChatRoleContent).toString();
    title = QString("%1(%2) %3 %4").arg(id,nick,data,time);
    painter->drawText(option.rect, title);
//    pa
}

QSize ChatListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(100, 200);
}
