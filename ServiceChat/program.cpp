#include "mainwindow.h"
#include "program.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QNetworkReply>
#include <QProcess>
#include <Tim/timtool.h>

time_t GetTime()
{
    time_t t;
    std::time(&t);
    return t;
}

bool QCopyFile(const QString &src, const QString &desDir, bool cover)
{
    if(src == desDir)
        return true;
    if(!QFile::exists(src))
        return false;
    return QFile::copy(src, desDir);
}

QRect GetRectFrom2Point(const QPoint &p1, const QPoint &p2)
{
    auto px = std::minmax(p1.x(), p2.x());
    auto py = std::minmax(p1.y(), p2.y());
    return QRect(QPoint(px.first, py.first), QPoint(px.second, py.second));
}

QRect InterSection2Rect(const QRect &r1, const QRect &r2)
{
    int xmax = std::max(r1.x(), r2.x());
    int ymax = std::max(r1.y(), r2.y());

    int xmin = std::min(r1.x() + r1.width(), r2.x() + r2.width());
    int ymin = std::min(r1.y() + r1.height(), r2.y() + r2.height());
    if(xmax <= xmin && ymax <= ymin)
        return QRect(xmax, ymax, xmin - xmax, ymin - ymax);
    return QRect();
}

QString UuidToStringEx()
{
    QString s = QUuid::createUuid().toString();
    s = s.remove("{").remove("}");
    return s;
}

void SetWidgetCentral(QWidget *w)
{
    w->move((qApp->desktop()->availableGeometry().width() - w->width()) / 2 + qApp->desktop()->availableGeometry().x(),
               (qApp->desktop()->availableGeometry().height() - w->height()) / 2 + qApp->desktop()->availableGeometry().y());
}

void Restart()
{
    CloseAll();
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList("--disable-web-security"));
}

void CloseAll()
{
    for(auto w : TimTool::Instance().getChatWindowMap())
        w->close();
    MainWindow::Instance().close();
}

void RequestOrderList()
{
    QUrl url("http://www.yunprint.com/api/cse49910p/myorders/uid/chengyong");
    QNetworkAccessManager *man = new QNetworkAccessManager;
    QObject::connect(man, &QNetworkAccessManager::finished, [=](QNetworkReply *reply){
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        DEBUG_VAR(statusCode);
        if(reply->error() == QNetworkReply::NoError)
        {
            QString orders = QString::fromUtf8(reply->readAll());
            OrderListStr(orders);
            OrderList();
        }
    });
    man->get(QNetworkRequest(url));
}


QString OrderListStr(QString orderList)
{
    static QString instance;
    if(!orderList.isEmpty())
        instance = orderList;
//    DEBUG_VAR(instance);
    return instance;
}


QList<Order_S> &OrderList()
{
    static QList<Order_S> instance;
    if(!instance.isEmpty())
        return instance;
    QString str = OrderListStr();
    if(str.isNull())
    {
        return instance;
    }
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(str.toUtf8(), &parseError);
    if(parseError.error == QJsonParseError::NoError)
    {
        if(document.isArray())
        {
            QJsonArray arr = document.array();
            for(QJsonValue val : arr)
            {
                QJsonObject obj = val.toObject();
                QString oid = obj.take("oid").toString();
                QString servid = obj.take("servid").toString();
                int filenum = obj.take("filenum").toInt();
                double totalamt = obj.take("totalamt").toDouble();
                instance.push_back({oid, servid, filenum, totalamt});

            }
        }
    }
    return instance;
}
