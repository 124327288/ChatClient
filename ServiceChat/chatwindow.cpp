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
    TimTool::Instance().UpdateChatWindowMap(otherId, this);
    setWindowTitle(tr("%1 - Session").arg(otherRemark));
    GetConversation();
    if(!TimTool::Instance().GetContentEX(otherId).isEmpty())
    {
        auto list = TimTool::Instance().GetContentEX(otherId);
        for(auto s : list)
        {
            AddContent(otherId, otherNick, s.time, s.text);
        }
    }
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
    QPalette pal = ui->textBrowser->palette();
    QColor c;
    pal.setColor(QPalette::Base, c.blue());
    ui->textBrowser->append(QString(R"(<font color="blue">%1(%2) %3</font>)").arg(id).arg(nick).arg(time));
    pal.setColor(QPalette::Base, c.black());
    ui->textBrowser->append(msg);
    ui->textBrowser->append("");
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
    AddContent(TimTool::Instance().getId(), TimTool::Instance().getNick(), 0, text);
    ui->textEdit->clear();
}
