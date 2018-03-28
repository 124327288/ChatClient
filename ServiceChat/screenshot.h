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

};

#endif // SCREENSHOT_H
