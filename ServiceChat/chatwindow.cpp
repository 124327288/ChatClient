#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "tim_msg_c.h"
#include "tim_c.h"
#include "chatlistmodel.h"
#include "chatlistdelegate.h"
#include "timtool.h"
#include <QDebug>
#include <QWaitCondition>
#include <Windows.h>
ChatWindow::ChatWindow(const Linkman &linkman, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    otherId = linkman.id;
    otherNick = !linkman.nick.trimmed().isEmpty() ? linkman.nick : otherId;
    otherRemark = !linkman.remark.trimmed().isEmpty() ? linkman.remark : otherNick;
    TimTool::Instance().AddChatWindowMap(otherId, this);
    setWindowTitle(tr("%1 - Session").arg(otherRemark));
//    ui->textBrowser->append();
    GetConversation();
}

ChatWindow::~ChatWindow()
{
    delete ui;
    TimTool::Instance().RemoveChatWindowMap(otherId);
    TimTool::Instance().RemoveConvMap(otherId);
    DestroyConversation(convHandle);
}

void ChatWindow::AddContent(QString id, QString nick, uint32_t time, QString msg)
{
    ui->textBrowser->append(QString("%1(%2) %3").arg(id).arg(nick).arg(time));
    ui->textBrowser->append(msg);
}

void ChatWindow::GetConversation()
{
    QByteArray bytes = otherId.toLatin1();
    const char* peer = bytes.data();
    convHandle = CreateConversation();
    TimTool::Instance().AddConvMap(otherId, convHandle);
    int rt = TIMGetConversation(convHandle, kCnvC2C, peer);
    if(rt == 0)
        qDebug() << "GetConversation Success!";
    else
        qDebug() << "GetConversation Error!";
}

void ChatWindow::on_sendBtn_clicked()
{
    QString text = ui->textEdit->toPlainText();
    TimTool::Instance().SendMsg(otherId, text);
    ui->textEdit->clear();
}
