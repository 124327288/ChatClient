#ifndef FILETABLEDELEGATE_H
#define FILETABLEDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>

class FileTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FileTableDelegate(QWidget *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // FILETABLEDELEGATE_H
