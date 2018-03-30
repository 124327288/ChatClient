#include "filetabledelegate.h"

#include <QPainter>

FileTableDelegate::FileTableDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}

void FileTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int col = index.column();
    if(col >= 0 && col <= 2)
    {
        QString s = index.data().toString();
        painter->drawText(option.rect, s);
    }
}

QSize FileTableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return {30, 30};
}
