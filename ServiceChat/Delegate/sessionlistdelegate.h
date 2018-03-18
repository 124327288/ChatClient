#ifndef SESSIONLISTDELEGATE_H
#define SESSIONLISTDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>
class SessionListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SessionListDelegate(QObject *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // SESSIONLISTDELEGATE_H
