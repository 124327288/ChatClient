#include "pdflistitemdelegate.h"

#include <QPainter>

PdfListItemDelegate::PdfListItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void PdfListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data(Qt::DisplayRole).canConvert<QImage>())
    {
        auto img = index.data(Qt::DisplayRole).value<QImage>();
        QPixmap pix = QPixmap::fromImage(img);
//        QPixmap pix(200, 200);
//        pix.fill(Qt::blue);
//        auto imgg = QImage::from
//        painter->drawImage(option.rect, img);
//        painter->drawImage(option.rect.x(), option.rect.y(), img);
        painter->drawPixmap(option.rect.x(), option.rect.y(), pix);
    }
}

QSize PdfListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return option.rect.size();
}
