#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "stdafx.h"
#include <QMainWindow>
#include <tim_conv_c.h>
namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(const Linkman &linkman, QWidget *parent = 0);
    ~ChatWindow();
    void AddContent(QString id, QString nick, time_t time, QString msg);
private slots:
    void on_sendBtn_clicked();

private:
    Ui::ChatWindow *ui;
    QString otherId;
    QString otherNick;
    QString otherRemark;

    TIMConversationHandle convHandle;

    void GetConversation();

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // CHATWINDOW_H
