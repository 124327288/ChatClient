#include "Delegate/filetableitemdelegate.h"

#include <QPainter>

FileTableItemDelegate::FileTableItemDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}

void FileTableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int col = index.column();
    if(col >= 0 && col <= 2)
    {
        QString s = index.data().toString();
        painter->drawText(option.rect, s);
    }
}

QSize FileTableItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return {30, 30};
}
