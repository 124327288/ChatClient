#include "webconnect.h"

WebConnect::WebConnect(QObject *parent) : QObject(parent)
{

}

void WebConnect::ReceiveText(const QString &text)
{
    qDebug() << QString("Receive message: %1").arg(text);
}
