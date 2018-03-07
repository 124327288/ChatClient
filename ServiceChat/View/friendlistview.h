#ifndef FRIENDLISTVIEW_H
#define FRIENDLISTVIEW_H

#include <QWidget>
#include <QListView>
class FriendListView : public QListView
{
    Q_OBJECT
public:
    explicit FriendListView(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // FRIENDLISTVIEW_H
