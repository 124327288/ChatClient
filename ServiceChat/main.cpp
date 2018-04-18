#include <QApplication>
#include <QWebEngineView>
#include "loginwindow.h"
#include "mainwindow.h"
#include "luatool.h"
#include "Protocol/tcpsocket.h"
#include "Tim/timtool.h"
#include "program.h"
#include "sqlitetool.h"
#include "emotiondialog.h"
#include "screenshot.h"
#include "filewindow.h"
#include <typeinfo>
#include <iostream>
#include "Table/table.h"
#include "databasetool.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Linkman>("Linkman");
    qRegisterMetaType<QList<Linkman>>("QList<Linkman>");
    qRegisterMetaType<uint32_t>("uint32_t");
    qRegisterMetaType<time_t>("time_t");

    auto loadGlobalTrans = [&a]{
        static QTranslator trans;
        trans.load(":/qt_zh_CN.qm");
        a.installTranslator(&trans);
    };
    auto loadMineTrans = [&a]{
        static QTranslator trans;
        if(LuaTool::Instance().getLanguage() == "en")
            trans.load(":/client_en.qm");
        else
            trans.load(":/client_cn.qm");
        a.installTranslator(&trans);
    };
    loadGlobalTrans();
    loadMineTrans();
    GenCacheDir();
    LuaTool::Instance().Init();
    LuaTool::Instance().getConfigs();
    TcpSocket::Instance().TryConnect();
    TimTool::Instance().Init();
    LoginWindow::Instance().show();
//    PDFWindow window;
//    window.show();
//    FileWindow window;
//    window.show();


    return a.exec();
}
