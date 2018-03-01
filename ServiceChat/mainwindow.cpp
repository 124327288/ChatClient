#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include "C2S/userpwdprotocol.h"
#include "S2C/signatureprotocol.h"
#include <thread>
#include "program.h"
#include <QCloseEvent>
#include "timtool.h"
#include "linkmanlistmodel.h"
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
    listModel = new LinkmanListModel;
    ui->listView->setModel(listModel);
    connect(&TimTool::Instance(), &TimTool::GetSelfNickname, this, [=](QString nick){
        ui->nickLabel->setText(nick.isEmpty()?TimTool::Instance().getId() : nick);
    });
    connect(ui->actionAboutQt, &QAction::triggered, [=](){
        QMessageBox::aboutQt(this);
    });
//    ui->listView->up
//    ui->listView->
//    connect(listModel, &ListModel::updated, ui->listView, &QListView::update);
//    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget, QStringList(QString("Root")));
//    ui->treeWidget->insertTopLevelItems();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
//    Program::Instance().state = PROGRAMSTATE::EXIT;
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
