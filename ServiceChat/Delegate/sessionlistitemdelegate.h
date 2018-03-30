#ifndef SESSIONLISTITEMDELEGATE_H
#define SESSIONLISTITEMDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>
class SessionLisItemtDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SessionLisItemtDelegate(QObject *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // SESSIONLISTITEMDELEGATE_H
