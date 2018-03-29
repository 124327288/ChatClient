#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QLabel>
#include <QObject>
#include <QWidget>
namespace ShotTool
{
    QRect GetScreenRect();
    constexpr int GetBright() { return 10; }
    constexpr int GetDim() { return -GetBright();}

    QImage GetScreenImage();
    QImage GetGrayScreenImage(int dim = GetDim());
    QImage GetNoGraySmallImage(const QImage &image, const QRect &rect, int bright = GetBright());

    QPixmap GetScreenPixmap();
    QPixmap GetGrayScrrenPixmap(int dim = GetDim());
    QPixmap GetNoGraySmallPixmap(const QPixmap &pixmap, const QRect &rect, int bright = GetBright());
}
class ScreenShot : public QWidget
{
    Q_OBJECT
private:
    ScreenShot(QWidget *parent = nullptr);
public:
    static ScreenShot &Instance();
    static void BeginShot();
signals:

public slots:

private:
    bool isInPaint;
    int bright = 10;

    QPoint fromPoint;
    QPoint toPoint;
    QLabel *label;

    QRect screenRect;
    QImage screenImage;
    QImage grayScreenImage;
    QImage noGraySmallImage;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

};

#endif // SCREENSHOT_H
