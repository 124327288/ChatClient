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
    static QRect instance;
    instance = QApplication::desktop()->rect();
    return instance;
}

const QPixmap &ScreenShot::GetScreenPixmap()
{
    static QPixmap instance;
    const QRect &rect = GetScreenRect();
    instance = QGuiApplication::primaryScreen()->grabWindow(0,
        rect.x(), rect.y(), rect.width(), rect.height());
    return instance;
}

const QPixmap &ScreenShot::GetGrayScrrenPixmap()
{
    static QPixmap instance;
    instance = GetScreenPixmap();
    QPainter painter(&instance);
    QPixmap grayPixmap(instance.width(), instance.height());
    grayPixmap.fill(QColor(0, 0, 0, 180));
    painter.drawPixmap(0, 0, grayPixmap);
    return instance;
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
    DEBUG_FUNC;
    if(event->button() == Qt::LeftButton)
    {
        isInPaint = true;
        fromPoint = event->pos();
    }
}

void ScreenShot::mouseReleaseEvent(QMouseEvent *event)
{
    DEBUG_FUNC;
    if(event->button() == Qt::RightButton)
        close();
}

void ScreenShot::mouseMoveEvent(QMouseEvent *event)
{
    DEBUG_FUNC;
    if(event->buttons() & Qt::LeftButton)
    {
        if(isInPaint)
        {
            toPoint = event->pos();
            repaint();
        }
    }

}

void ScreenShot::paintEvent(QPaintEvent *event)
{
    DEBUG_FUNC;
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
