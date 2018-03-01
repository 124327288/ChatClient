#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "exptype.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(const Linkman &linkman, QWidget *parent = 0);
    ~ChatWindow();

private:
    Ui::ChatWindow *ui;
    QString otherId;
    QString otherNick;
    QString otherRemark;
};

#endif // CHATWINDOW_H
