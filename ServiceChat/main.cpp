#include "loginwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include "luatool.h"
#include "tcpsocket.h"
#include "timtool.h"
#include "program.h"
#include "exptype.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator trans;
    trans.load(":/res/client_cn");
    a.installTranslator(&trans);
    Program::Instance().state = PROGRAMSTATE::NOPAGE;

    qRegisterMetaType<Linkman>("Linkman");
    qRegisterMetaType<QList<Linkman>>("QList<Linkman>");
    qRegisterMetaType<uint32_t>("uint32_t");

    LuaTool::Instance().Init();
    LuaTool::Instance().getConfigs();
    TcpSocket::Instance().TryConnect();
    TimTool::Instance().Init();
    TimTool::Instance().SetMessageCallback();
    LoginWindow::Instance().show();

    Program::Instance().state = PROGRAMSTATE::LOGINPAGE;
    return a.exec();
}
