#include "screenshot.h"

#include <QApplication>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>
#include <QCloseEvent>
#include <QRgb>
#include <cmath>
#include "stdafx.h"
using namespace ShotTool;
ScreenShot::ScreenShot(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    label = new QLabel(this);
    screenRect = GetScreenRect();
    screenImage = GetScreenImage();
    grayScreenImage = GetGrayScreenImage();
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(grayScreenImage));
    setPalette(pal);
    setAutoFillBackground(true);
    setGeometry(screenRect);
    showFullScreen();

}

ScreenShot &ScreenShot::Instance()
{
    static ScreenShot instance;
    return instance;
}

void ScreenShot::BeginShot()
{
    ScreenShot::Instance().show();
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
//    QString s = QString("%1, %2").arg(event->pos().x()).arg(event->pos().y());
//    label->setText(s);
//    label->setGeometry(event->pos().x(), event->pos().y(), 200, 100);
//    DEBUG_FUNC;
    if(event->buttons() & Qt::LeftButton)
    {
        if(isInPaint)
        {
            toPoint = event->pos();
            QRect rect = GetRectFrom2Point(fromPoint, toPoint);
            noGraySmallImage = GetNoGraySmallImage(grayScreenImage, rect);
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
        QRect rect = GetRectFrom2Point(fromPoint, toPoint);
        painter.drawImage(rect, noGraySmallImage);
        painter.drawRect(rect);
        painter.end();
    }
    QWidget::paintEvent(event);

}

QRect ShotTool::GetScreenRect()
{
    return QApplication::desktop()->rect();
}

QPixmap ShotTool::GetScreenPixmap()
{
    QRect rect = GetScreenRect();
    return QGuiApplication::primaryScreen()->grabWindow(0,
        rect.x(), rect.y(), rect.width(), rect.height());
}

QPixmap ShotTool::GetGrayScrrenPixmap(int dim)
{
    QImage img = GetScreenPixmap().toImage();
    for(int y = 0; y < img.height(); ++y)
    for(int x = 0; x < img.width(); ++x)
    {
        QColor oc = img.pixelColor(x, y);
        int r = oc.red();
        int g = oc.green();
        int b = oc.blue();
        int r2 = Range(r + dim, 0, 255);
        int g2 = Range(g + dim, 0, 255);
        int b2 = Range(b + dim, 0, 255);
        img.setPixel(x, y, qRgb(r2, g2, b2));
    }
    return QPixmap::fromImage(img);
}

QPixmap ShotTool::GetNoGraySmallPixmap(const QPixmap &pixmap, const QRect &rect, int bright)
{
    QImage oimg = pixmap.toImage();
    QImage img(QSize(rect.width(), rect.height()), QImage::Format_ARGB32);
    for(int j = 0; j < rect.height(); ++j)
    for(int i = 0; i < rect.width(); ++i)
    {
        int x = i + rect.x();
        int y = j + rect.y();
        QColor oc = oimg.pixelColor(x, y);
        int r = oc.red();
        int g = oc.green();
        int b = oc.blue();
        int r2 = Range(r + bright, 0, 255);
        int g2 = Range(g + bright, 0, 255);
        int b2 = Range(b + bright, 0, 255);
        img.setPixel(i + 1, j + 1, qRgb(r2, g2, b2));
    }
    return QPixmap::fromImage(img);
}

QImage ShotTool::GetScreenImage()
{
    return GetScreenPixmap().toImage();
}

QImage ShotTool::GetGrayScreenImage(int dim)
{
    auto img = GetScreenImage();
    for(int y = 0; y < img.height(); ++y)
    for(int x = 0; x < img.width(); ++x)
    {
        QColor oc = img.pixelColor(x, y);
        int r = oc.red();
        int g = oc.green();
        int b = oc.blue();
        int r2 = Range(r + dim, 0, 255);
        int g2 = Range(g + dim, 0, 255);
        int b2 = Range(b + dim, 0, 255);
        img.setPixel(x, y, qRgb(r2, g2, b2));
    }
    return img;
}

QImage ShotTool::GetNoGraySmallImage(const QImage &image, const QRect &rect, int bright)
{
    QImage img = image.copy(rect);
//    return img;
    for(int j = 0; j < rect.height(); ++j)
    for(int i = 0; i < rect.width(); ++i)
    {
        QColor oc = img.pixelColor(i, j);
        int r = oc.red();
        int g = oc.green();
        int b = oc.blue();
        int r2 = Range(r + bright, 0, 255);
        int g2 = Range(g + bright, 0, 255);
        int b2 = Range(b + bright, 0, 255);
        img.setPixel(i, j, qRgb(r2, g2, b2));
    }
    return img;
}
