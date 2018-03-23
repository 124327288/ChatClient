﻿#include <QApplication>
#include "loginwindow.h"
#include "mainwindow.h"
#include "pdfwindow.h"
#include "luatool.h"
#include "Protocol/tcpsocket.h"
#include "Tim/timtool.h"
#include "program.h"
#include "sqlitetool.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Linkman>("Linkman");
    qRegisterMetaType<QList<Linkman>>("QList<Linkman>");
    qRegisterMetaType<uint32_t>("uint32_t");

    LuaTool::Instance().Init();
    LuaTool::Instance().getConfigs();
    QTranslator trans;
    if(LuaTool::Instance().getLanguage() == "en")
        trans.load(":/client_en.qm");
    else
        trans.load(":/client_cn.qm");
    a.installTranslator(&trans);
    TcpSocket::Instance().TryConnect();
    TimTool::Instance().Init();
    TimTool::Instance().SetMessageCallback();
//    PDFWindow window;
//    window.show();
    SqliteTool::Instance().CreateConnect();
    LoginWindow::Instance().show();

//    SqliteTool::Instance().CreateAccountTable();
//    SqliteTool::Instance().Insert2AccountTable("123","456","789");
//    SqliteTool::Instance().Select4AccountTable("123");

    return a.exec();
}
