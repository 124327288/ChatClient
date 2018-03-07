#ifndef CHATLISTDELEGATE_H
#define CHATLISTDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>

class ChatListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ChatListDelegate(QObject *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // CHATLISTDELEGATE_H
