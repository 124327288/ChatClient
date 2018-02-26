#include "booklistdelegate.h"

BookListDelegate::BookListDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}

void BookListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;
    QString name = index.data(Qt::DisplayRole).toString();

}

QSize BookListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(80);
    return size;
}
