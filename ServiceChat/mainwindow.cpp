#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QCloseEvent>
#include "program.h"
#include "Protocol/C2S/userpwdprotocol.h"
#include "Protocol/S2C/signatureprotocol.h"
#include "Tim/timtool.h"
#include "Model/friendlistmodel.h"
#include "Delegate/friendlistdelegate.h"
#include "chatwindow.h"
MainWindow &MainWindow::Instance()
{
    static MainWindow instance;
    return instance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();
    friendListModel = new FriendListModel;
    friendListView->setModel(friendListModel);
    friendListView->setItemDelegate(new FriendListDelegate);
    connect(&TimTool::Instance(), &TimTool::GetSelfNickname, this, [=](QString nick){
        ui->nickLabel->setText(nick.isEmpty()?TimTool::Instance().getId() : nick);
    });
    connect(ui->actionAboutQt, &QAction::triggered, [=](){
        QMessageBox::aboutQt(this);
    });

    connect(friendListView, &QListView::doubleClicked, [=](const QModelIndex &index){

        QString id = index.data(Role::RoleID).toString();
        QString nick = index.data(Role::RoleNick).toString();
        QString remark = index.data(Role::RoleRemark).toString();
        ChatWindow *window;
        if(TimTool::Instance().ContainInChatWindowMap(id))
        {
            window = TimTool::Instance().GetChatWindow(id);
            window->activateWindow();
        }
        else
        {
            window = new ChatWindow({id, nick, remark}, this);
            window->show();
        }

    });
}

void MainWindow::InitUI()
{
    QWidget *page1 = new QWidget;
    QVBoxLayout *vLayout1 = new QVBoxLayout;
    sessionListView = new QListView;
    vLayout1->addWidget(sessionListView);
    page1->setLayout(vLayout1);
    ui->tabWidget->addTab(page1, tr("Session"));

    QWidget *page2 = new QWidget;
    QVBoxLayout *vLayout2 = new QVBoxLayout;
    friendListView = new QListView;
    vLayout2->addWidget(friendListView);
    page2->setLayout(vLayout2);
    ui->tabWidget->addTab(page2, tr("Friend"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::on_addFriendBtn_clicked()
{
   QString id = QInputDialog::getText(this, "Input Friend ID", "Friend ID");
   if(id.trimmed().isEmpty())
    return;
   TimTool::Instance().AddSingleFriend(id, "hehe");

}

void MainWindow::updateListView()
{

}
