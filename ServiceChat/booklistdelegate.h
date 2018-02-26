#ifndef BOOKLISTDELEGATE_H
#define BOOKLISTDELEGATE_H
#include <QStyledItemDelegate>
#include <QWidget>
#include <QIcon>
class BookListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    BookListDelegate(QWidget *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    QPixmap head;
};

#endif // BOOKLISTDELEGATE_H
