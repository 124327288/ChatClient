#ifndef FILETABLEITEMDELEGATE_H
#define FILETABLEITEMDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>

class FileTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FileTableItemDelegate(QWidget *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // FILETABLEITEMDELEGATE_H
