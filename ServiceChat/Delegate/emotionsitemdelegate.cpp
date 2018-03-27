#include "emotionsitemdelegate.h"

#include <QPainter>

EmotionsItemDelegate::EmotionsItemDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}

void EmotionsItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data(Qt::DisplayRole).canConvert<QPixmap>())
    {
        QPixmap pixmap = index.data(Qt::DisplayRole).value<QPixmap>().scaled(30, 30);
        painter->drawPixmap(option.rect, pixmap);
    }
    QStyledItemDelegate::paint(painter, option, index);
}
