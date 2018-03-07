#include <QApplication>
#include "loginwindow.h"
#include "mainwindow.h"
#include "pdfwindow.h"
#include "luatool.h"
#include "Protocol/tcpsocket.h"
#include "Tim/timtool.h"
#include "program.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator trans;
    trans.load(":/res/client_cn");
    a.installTranslator(&trans);

    qRegisterMetaType<Linkman>("Linkman");
    qRegisterMetaType<QList<Linkman>>("QList<Linkman>");
    qRegisterMetaType<uint32_t>("uint32_t");

//    Program program;
//    program.setProgram(&program);

    LuaTool::Instance().Init();
    LuaTool::Instance().getConfigs();
    TcpSocket::Instance().TryConnect();
    TimTool::Instance().Init();
    TimTool::Instance().SetMessageCallback();
//    PDFWindow window;
//    window.show();
    LoginWindow::Instance().show();

    return a.exec();
}
