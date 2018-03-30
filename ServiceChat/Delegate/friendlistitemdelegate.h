#ifndef FRIENDLISTITEMDELEGATE_H
#define FRIENDLISTITEMDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>
class FriendListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FriendListItemDelegate(QObject *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // FRIENDLISTITEMDELEGATE_H
