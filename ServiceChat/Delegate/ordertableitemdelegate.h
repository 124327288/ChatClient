#ifndef ORDERTABLEITEMDELEGATE_H
#define ORDERTABLEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>

class OrderTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit OrderTableItemDelegate(QWidget *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // ORDERTABLEITEMDELEGATE_H
