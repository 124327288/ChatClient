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
    TimTool::Instance().chatMap[otherId] = this;
    setWindowTitle(tr("%1 - Session").arg(otherRemark));
//    ui->textBrowser->append();
    GetConversation();
}

ChatWindow::~ChatWindow()
{
    delete ui;
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
    int rt = TIMGetConversation(convHandle, kCnvC2C, peer);
    if(rt == 0)
        qDebug() << "GetConversation Success!";
    else
        qDebug() << "GetConversation Error!";
}

void ChatWindow::on_sendBtn_clicked()
{
//    void SendMsg(TIMConversationHandle conv_handle, TIMMessageHandle msg_handle, TIMCommCB *callback);
//    typedef void* TIMMsgTextElemHandle;
//    TIMMsgTextElemHandle    CreateMsgTextElem();

//    void    SetContent(TIMMsgTextElemHandle handle, const char* content);
//    uint32_t    GetContentLen(TIMMsgTextElemHandle handle);
//    int    GetContent(TIMMsgTextElemHandle handle, char* content, uint32_t* len);
    TIMMessageHandle msgHandle = CreateTIMMessage();
    TIMMsgTextElemHandle txtHandle = CreateMsgTextElem();
    QString text = ui->textEdit->toPlainText();
    QByteArray bytes = text.toLatin1();
    qDebug() << "send: " << bytes.data();
    SetContent(txtHandle, bytes.data());
    AddElem(msgHandle, txtHandle);
    ui->textEdit->clear();
    qDebug() << "send: " << bytes.data();
    TIMCommCB cb;
    cb.OnSuccess = [](void*){
        qDebug() << "OnSuccess!";
    };
    cb.OnError = [](int code, const char* desc, void* data){
        qDebug() << QString("OnError! code = %1, desc = %2").arg(code).arg(desc);
    };
    cb.data = &cb;
    SendMsg(convHandle, msgHandle, &cb);
    Sleep(1);
    DestroyElem(txtHandle);
    DestroyTIMMessage(msgHandle);
}
