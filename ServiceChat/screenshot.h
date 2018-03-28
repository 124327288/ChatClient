#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QObject>
#include <QWidget>

class ScreenShot : public QWidget
{
    Q_OBJECT
private:
    ScreenShot(QWidget *parent = nullptr);
    const QRect &GetScreenRect();
    const QPixmap &GetScreenPixmap();
    const QPixmap &GetGrayScrrenPixmap();//渡灰
    QPixmap GetNoGrayPixmap(const QPixmap &pixmap, const QRect &rect);
    QRect GetRectFrom2Point(const QPoint &p1, const QPoint &p2);
public:
    static ScreenShot &Instance();
    static void BeginShot();

signals:

public slots:

private:
    bool isInPaint;
    QRect screenRect;
    QPoint fromPoint;
    QPoint toPoint;

    QPixmap screenPixmap;
    QPixmap grayScrrenPixmap;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

};

#endif // SCREENSHOT_H
