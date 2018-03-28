#include "screenshot.h"

#include <QApplication>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>
#include <QCloseEvent>
#include <cmath>
#include "stdafx.h"

ScreenShot::ScreenShot(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    const QPixmap &pixmap = GetGrayScrrenPixmap();
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(pixmap));
    setPalette(pal);
    setAutoFillBackground(true);
    setGeometry(GetScreenRect());
    showFullScreen();
}

const QRect &ScreenShot::GetScreenRect()
{
    if(!screenRect.isNull())
        return screenRect;
    screenRect = QApplication::desktop()->rect();
    return screenRect;
}

const QPixmap &ScreenShot::GetScreenPixmap()
{
    if(!screenPixmap.isNull())
        return screenPixmap;
    const QRect &rect = GetScreenRect();
    screenPixmap = QGuiApplication::primaryScreen()->grabWindow(0,
        rect.x(), rect.y(), rect.width(), rect.height());
    return screenPixmap;
}

const QPixmap &ScreenShot::GetGrayScrrenPixmap()
{
    if(!grayScrrenPixmap.isNull())
        return grayScrrenPixmap;
    grayScrrenPixmap = GetScreenPixmap();
    QPainter painter(&grayScrrenPixmap);
    QPixmap grayPixmap(grayScrrenPixmap.width(), grayScrrenPixmap.height());
    grayPixmap.fill(QColor(0, 0, 0, 180));
    painter.drawPixmap(0, 0, grayPixmap);
    return grayScrrenPixmap;
}

QPixmap ScreenShot::GetNoGrayPixmap(const QPixmap &pixmap, const QRect &rect)
{
    QPixmap instance = pixmap;
    QPainter p(&instance);
    QPixmap noGrayPixmap(rect.width(), rect.height());
    noGrayPixmap.fill(QColor(0, 0, 0, 100));
    p.drawPixmap(rect.x(), rect.y(), noGrayPixmap);
    return instance;
}

QRect ScreenShot::GetRectFrom2Point(const QPoint &p1, const QPoint &p2)
{
    auto px = std::minmax(p1.x(), p2.x());
    auto py = std::minmax(p1.y(), p2.y());
    return QRect(QPoint(px.first, py.first), QPoint(px.second, py.second));
}

ScreenShot &ScreenShot::Instance()
{
    static ScreenShot instance;
    return instance;
}

void ScreenShot::BeginShot()
{
    ScreenShot::Instance().show();
    //    show();
}

void ScreenShot::mousePressEvent(QMouseEvent *event)
{
//    DEBUG_FUNC;
    if(event->button() == Qt::LeftButton)
    {
        isInPaint = true;
        fromPoint = event->pos();
    }
}

void ScreenShot::mouseReleaseEvent(QMouseEvent *event)
{
//    DEBUG_FUNC;
    if(event->button() == Qt::RightButton)
        close();
}

void ScreenShot::mouseMoveEvent(QMouseEvent *event)
{
//    DEBUG_FUNC;
    if(event->buttons() & Qt::LeftButton)
    {
        if(isInPaint)
        {
            toPoint = event->pos();
            QPixmap pixmap = GetNoGrayPixmap(GetGrayScrrenPixmap(), GetRectFrom2Point(toPoint, fromPoint));
            QPalette pal(palette());
            pal.setBrush(QPalette::Window, QBrush(pixmap));
            setPalette(pal);
            repaint();
        }
    }

}

void ScreenShot::paintEvent(QPaintEvent *event)
{
//    DEBUG_FUNC;
    if(isInPaint)
    {
        QPainter painter;
        painter.begin(this);
        painter.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));
//        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        int x1,x2,y1,y2;

        auto px = std::minmax(fromPoint.x(), toPoint.x());
        x1 = px.first;
        x2 = px.second;

        auto py = std::minmax(fromPoint.y(), toPoint.y());
        y1 = py.first;
        y2 = py.second;
        painter.drawRect(x1, y1, x2 - x1, y2 - y1);
        painter.end();
    }
    QWidget::paintEvent(event);

}
