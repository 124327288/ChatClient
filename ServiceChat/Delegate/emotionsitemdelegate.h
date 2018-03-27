#ifndef EMOTIONSITEMDELEGATE_H
#define EMOTIONSITEMDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>
class EmotionsItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit EmotionsItemDelegate(QWidget *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // EMOTIONSITEMDELEGATE_H
