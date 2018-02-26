#include "bookitem.h"
#include <QGridLayout>
BookItem::BookItem(const QString &name, const QString &time, const QString &content, QWidget *parent) : QWidget(parent)
{
    nameLabel = new QLabel(name);
    timeLabel = new QLabel(time);
    contentLabel = new QLabel(content);
    layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(timeLabel, 0, 1);
    layout->addWidget(contentLabel, 1, 0);
}
