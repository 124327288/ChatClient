#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtNetwork>
#include <QDebug>
#include "C2S/userpwdprotocol.h"
#include "S2C/signatureprotocol.h"
#include <thread>
#include "program.h"
#include <QCloseEvent>
#include "timtool.h"
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
