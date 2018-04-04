#include <QApplication>
#include <QWebEngineView>
#include "loginwindow.h"
#include "mainwindow.h"
#include "pdfwindow.h"
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
//    std::cout << typeid(loadMineTrans).name();
    loadGlobalTrans();
    loadMineTrans();
    GenCacheDir();
    LuaTool::Instance().Init();
    LuaTool::Instance().getConfigs();
    TcpSocket::Instance().TryConnect();
    TimTool::Instance().Init();
    DatabaseTool dbTool;
    dbTool.Bind("Chat.db");
    Id id;
    id.setId("asdqwe");
    dbTool.Insert(id);
//    SqliteTool::Instance().CreateConnect();
    SqliteTool::Instance().Init();
    SqliteTool::Instance().ShowAllTableName();
//    Id id;
//    Account acc;
//    const QMetaObject *metaObject = acc.metaObject();
//    {
//        int cnt = metaObject->methodCount();
//        qDebug() << "\n";
//        for(int i = 0;i < cnt; ++i)
//        {
//            QMetaMethod method = metaObject->method(i);
//            DEBUG_VAR(method.methodSignature());
//            DEBUG_VAR(method.typeName());
//            DEBUG_VAR(method.methodType());
//            qDebug() << "method.parameterNames()" << method.parameterNames();
//            qDebug() << "\n";
//        }
//    }
//    {
//        int cnt = metaObject->propertyCount();
//        qDebug() << "\n";
//        for(int i = 0;i < cnt; ++i)
//        {
//            QMetaProperty property = metaObject->property(i);
//            DEBUG_VAR(property.name());
//            DEBUG_VAR(property.type());
//            DEBUG_VAR(property.typeName());
//            qDebug() << "\n";
//        }
//    }
    LoginWindow::Instance().show();
//    FileWindow window;
//    window.show();
//    ScreenShot::Instance().BeginShot();


    return a.exec();
}
