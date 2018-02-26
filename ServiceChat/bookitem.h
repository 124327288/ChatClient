#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QGridLayout>
class BookItem : public QWidget
{
    Q_OBJECT
public:
    explicit BookItem(const QString &name, const QString &time, const QString &content, QWidget *parent = nullptr);

signals:

public slots:

private:
    QLabel *nameLabel;
    QLabel *contentLabel;
    QLabel *timeLabel;
    QGridLayout *layout;
};

#endif // BOOKITEM_H
