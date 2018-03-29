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
#include <QFileDialog>
#include "stdafx.h"
using namespace ShotTool;
ScreenShot::ScreenShot(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    label = new QLabel(this);
    toolBar = new ToolBar(this);
    toolBar->setVisible(false);
    screenRect = GetScreenRect();
    screenImage = GetScreenImage();
    grayScreenImage = GetGrayScreenImage();
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(grayScreenImage));
    setPalette(pal);
    setAutoFillBackground(true);
    setGeometry(screenRect);
    showFullScreen();
    connect(toolBar, &ToolBar::SaveImage, this, &ScreenShot::SaveImage);
    connect(toolBar, &ToolBar::ExitShot, this, &ScreenShot::ExitShot);
    connect(toolBar, &ToolBar::OkShot, this, &ScreenShot::OkShot);
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

void ScreenShot::SaveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images Files(*.jpg *.png)"));
    if(fileName.isNull())
        return;
    noGraySmallImage.save(fileName);
    close();
}

void ScreenShot::ExitShot()
{
    close();
}

void ScreenShot::OkShot()
{
    QApplication::clipboard()->setImage(noGraySmallImage);
    close();
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
    {
        close();
        return;
    }
    toolBar->setVisible(true);
    int x = event->pos().x();
    int y = event->pos().y();
    int w = toolBar->size().width();
    int h = toolBar->size().height();
    toolBar->setGeometry(x, y, w, h);

}

void ScreenShot::mouseMoveEvent(QMouseEvent *event)
{
    QString s = QString("%1, %2").arg(event->pos().x()).arg(event->pos().y());
    label->setText(s);
    int x = event->pos().x();
    int y = event->pos().y();
    int w = 50;
    int h = label->size().height();
    label->setGeometry(x, y, w, h);
    if(event->buttons() & Qt::LeftButton)
    {
        if(isInPaint)
        {
            toPoint = event->pos();
            QRect rect = GetRectFrom2Point(fromPoint, toPoint);
            noGraySmallImage = GetNoGraySmallImage(grayScreenImage, rect);
        }
    }
    repaint();
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

QPixmap ShotTool::GetGrayScrrenPixmap(double dim)
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

QPixmap ShotTool::GetNoGraySmallPixmap(const QPixmap &pixmap, const QRect &rect, double bright)
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

QImage ShotTool::GetGrayScreenImage(double dim)
{
    auto img = GetScreenImage();
    for(int y = 0; y < img.height(); ++y)
    for(int x = 0; x < img.width(); ++x)
    {
        QColor oc = img.pixelColor(x, y);
        int r = oc.red();
        int g = oc.green();
        int b = oc.blue();
        int r2 = Range(r * (1 + dim), 0, 255);
        int g2 = Range(g * (1 + dim), 0, 255);
        int b2 = Range(b * (1 + dim), 0, 255);
        img.setPixel(x, y, qRgb(r2, g2, b2));
    }
    return img;
}

QImage ShotTool::GetNoGraySmallImage(const QImage &image, const QRect &rect, double bright)
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
        int r2 = Range(r / (1 - bright), 0, 255);
        int g2 = Range(g / (1 - bright), 0, 255);
        int b2 = Range(b / (1 - bright), 0, 255);
        img.setPixel(i, j, qRgb(r2, g2, b2));
    }
    return img;
}

ToolBar::ToolBar(QWidget *parent): QWidget(parent)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::blue);
    setAutoFillBackground(true);
    setPalette(pal);
    saveBtn = new QToolButton;
    saveBtn->setText(tr("Save"));
    exitBtn = new QToolButton;
    exitBtn->setText(tr("Exit"));
    okBtn = new QToolButton;
    okBtn->setText(tr("Ok"));
    mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(saveBtn);
    mainLayout->addWidget(exitBtn);
    mainLayout->addWidget(okBtn);
    setLayout(mainLayout);
    connect(saveBtn, &QToolButton::clicked, this, [=]{emit SaveImage();});
    connect(exitBtn, &QToolButton::clicked, this, [=]{emit ExitShot();});
    connect(okBtn, &QToolButton::clicked, this, [=]{emit OkShot();});
}

//ToolBar &ToolBar::Instance()
//{
//    static ToolBar instance;
//    return instance;
//}
