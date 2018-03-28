#include "screenshot.h"

#include <QApplication>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>

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
