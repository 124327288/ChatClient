#include "webconnect.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QMessageBox>
#include "stdafx.h"

WebConnect::WebConnect(QObject *parent) : QObject(parent)
{

}

WebConnect &WebConnect::Instance()
{
    static WebConnect instance;
    return instance;
}

void WebConnect::ReceiveText(const QString &text)
{
    QMessageBox::information(nullptr, "Title", "C++ And Js");
    qDebug() << QString("Receive message: %1").arg(text);
}

void WebConnect::OpenFolder(const QString &path)
{
    QDesktopServices::openUrl(QUrl(QString("file:") + path, QUrl::TolerantMode));
}

void WebConnect::OpenFile(const QString &path)
{
    DEBUG_VAR(path);
    DEBUG_VAR(qApp->applicationDirPath());
    if(path.right(4).toLower() == ".pdf")
    {
        QString fullPath = qApp->applicationDirPath() + "/Mini-Pdf-Reader.exe " + path;
//        POP_INFOMATION(fullPath);
        QProcess::startDetached(fullPath);
        return;
    }
    QDesktopServices::openUrl(QUrl(QString("file:") + path, QUrl::TolerantMode));
}
