#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QObject>
#include <QWidget>

class ScreenShot : public QWidget
{
    Q_OBJECT
private:
    ScreenShot(QWidget *parent = nullptr);
    static const QRect &GetScreenRect();
    static const QPixmap &GetScreenPixmap();
    static const QPixmap &GetGrayScrrenPixmap();//渡灰
public:
    static ScreenShot &Instance();
    static void BeginShot();

signals:

public slots:

private:
    bool isInPaint;
    QPoint fromPoint;
    QPoint toPoint;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // SCREENSHOT_H
