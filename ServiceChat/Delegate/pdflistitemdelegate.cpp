#include "pdflistitemdelegate.h"

#include <QPainter>

PdfListItemDelegate::PdfListItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void PdfListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto img = index.data(Qt::DisplayRole).value<QImage>();
    painter->drawImage(option.rect, img);
}

QSize PdfListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return option.rect.size();
}
