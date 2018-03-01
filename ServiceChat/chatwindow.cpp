#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(const Linkman &linkman, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    otherId = linkman.id;
    otherNick = !linkman.nick.trimmed().isEmpty() ? linkman.nick : otherId;
    otherRemark = !linkman.remark.trimmed().isEmpty() ? linkman.remark : otherNick;
    setWindowTitle(tr("%1 - Session").arg(otherRemark));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}
