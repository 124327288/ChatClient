#include "webconnect.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QMessageBox>
#include "pdfwindow.h"
#include "stdafx.h"

WebConnect::WebConnect(QObject *parent) : QObject(parent)
{

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
    if(path.right(4).toLower() == ".pdf")
    {
        PDFWindow *window = new PDFWindow;
        window->ShowPDF(path);
        SetWidgetCentral(window);
        window->show();
        return;
    }
    QDesktopServices::openUrl(QUrl(QString("file:") + path, QUrl::TolerantMode));
}
