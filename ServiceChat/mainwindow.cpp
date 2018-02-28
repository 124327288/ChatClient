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
#include "listmodel.h"
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
    ui->listView->setModel(new ListModel);
//    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget, QStringList(QString("Root")));
//    ui->treeWidget->insertTopLevelItems();
}

MainWindow::~MainWindow()
{
    delete ui;
}
QTcpSocket *socket = new QTcpSocket();


void MainWindow::on_myPushButton_clicked()
{
    TimTool::Instance().AddSingleFriend("ylzf0000", "");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
//    Program::Instance().state = PROGRAMSTATE::EXIT;
    event->accept();
}

void MainWindow::on_addFriendBtn_clicked()
{
   QString id = QInputDialog::getText(this, "Input Friend ID", "Input Friend ID");
   if(id.trimmed().isEmpty())
    return;
   TimTool::Instance().AddSingleFriend(id, "hehe");

}
