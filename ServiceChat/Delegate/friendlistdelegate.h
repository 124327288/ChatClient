#ifndef FRIENDLISTDELEGATE_H
#define FRIENDLISTDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>
class FriendListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FriendListDelegate(QObject *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // FRIENDLISTDELEGATE_H
