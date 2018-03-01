#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "exptype.h"
#include "tim_conv_c.h"
namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(const Linkman &linkman, QWidget *parent = 0);
    ~ChatWindow();

private slots:
    void on_sendBtn_clicked();

private:
    Ui::ChatWindow *ui;
    QString otherId;
    QString otherNick;
    QString otherRemark;

    TIMConversationHandle convHandle;

    void GetConversation();
};

#endif // CHATWINDOW_H
