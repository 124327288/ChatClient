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
#include "Delegate/sessionlistdelegate.h"
#include "settingdialog.h"
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
    sessionListModel = new SessionListModel;
    sessionListView->setModel(sessionListModel);
    sessionListView->setItemDelegate(new SessionListDelegate);
    friendListModel = new FriendListModel;
    friendListView->setModel(friendListModel);
    friendListView->setItemDelegate(new FriendListDelegate);

    connect(&TimTool::Instance(), &TimTool::GetSelfNickname, this, &MainWindow::SetNickName);
    connect(ui->actionAboutQt, &QAction::triggered, [=]{QMessageBox::aboutQt(this);});
    connect(friendListView, &QListView::doubleClicked, this, &MainWindow::PopChatWindow);
	connect(sessionListView, &QListView::doubleClicked, this, &MainWindow::PopChatWindow);
    connect(friendListView, &QListView::customContextMenuRequested,[]{});
    connect(ui->actionSetting, &QAction::triggered, [=]{auto dialog = new SettingDialog(this);dialog->show();});
    TimTool::Instance().GetSelfProfile();
    TimTool::Instance().GetFriendList();
}

void MainWindow::InitUI()
{
	if (!TimTool::Instance().getId().isEmpty() && !TimTool::Instance().getNick().isEmpty())
	{
		QString title = QString("%1(%2)").arg(TimTool::Instance().getId()).arg(TimTool::Instance().getNick());
		setWindowTitle(title);
		ui->nickLabel->setText(TimTool::Instance().getNick().isEmpty() ? TimTool::Instance().getId() : TimTool::Instance().getNick());
	}
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

void MainWindow::SetNickName(const QString & nick)
{
	QString title = QString("%1(%2)").arg(TimTool::Instance().getId()).arg(TimTool::Instance().getNick());
	setWindowTitle(title);
	ui->nickLabel->setText(nick.isEmpty() ? TimTool::Instance().getId() : nick);
}

void MainWindow::PopChatWindow(const QModelIndex & index)
{
	QString id = index.data(Role::RoleID).toString();
	QString nick = index.data(Role::RoleNick).toString();
	ChatWindow *window;
	if (TimTool::Instance().ContainInChatWindowMap(id))
	{
		window = TimTool::Instance().GetChatWindow(id);
		window->activateWindow();
	}
	else
	{
        window = new ChatWindow({ id, nick }, this);
		window->show();
	}
}

MainWindow::~MainWindow()
{
    if(ui)
    {
        delete ui;
        ui = nullptr;
    }
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
