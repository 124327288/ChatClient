#include "webconnect.h"

#include <QMessageBox>

WebConnect::WebConnect(QObject *parent) : QObject(parent)
{

}

void WebConnect::ReceiveText(const QString &text)
{
    QMessageBox::information(nullptr, "Title", "C++ And Js");
    qDebug() << QString("Receive message: %1").arg(text);
}
