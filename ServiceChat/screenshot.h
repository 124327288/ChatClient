#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QBoxLayout>
#include <QLabel>
#include <QObject>
#include <QToolButton>
#include <QWidget>
namespace ShotTool
{
    QRect GetScreenRect();
    constexpr double GetBright() { return 0.5; }
    constexpr double GetDim() { return -GetBright();}

    QImage GetScreenImage();
    QImage GetGrayScreenImage(double dim = GetDim());
    QImage GetNoGraySmallImage(const QImage &image, const QRect &rect, double bright = GetBright());
    QImage GetNoGraySmallImageFromLast(const QImage &image, const QImage &lastImg, const QRect &lastRect, const QRect &interRect, const QRect &rect, double bright = GetBright());

    QPixmap GetScreenPixmap();
    QPixmap GetGrayScrrenPixmap(double dim = GetDim());
    QPixmap GetNoGraySmallPixmap(const QPixmap &pixmap, const QRect &rect, double bright = GetBright());
}
class ChatWindow;
class ToolBar;
class ScreenShot : public QWidget
{
    Q_OBJECT
public:
    ScreenShot(QWidget *parent = nullptr);
public:
//    static ScreenShot &Instance();
    static void BeginShot();
    ChatWindow *getChatWindow() const;
    void setChatWindow(ChatWindow *value);

signals:
    void HasImageInClipboard();
public slots:
    void SaveImage();
    void ExitShot();
    void OkShot();
private:
    bool isInPaint;

    QPoint fromPoint;
    QPoint toPoint;
    QLabel *label;
    ToolBar *toolBar;

    QRect screenRect;
    QImage screenImage;
    QImage grayScreenImage;
    QImage noGraySmallImage;

    ChatWindow *chatWindow = nullptr;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
};

class ToolBar : public QWidget
{
    Q_OBJECT
public:
    ToolBar(QWidget *parent = nullptr);
signals:
    void SaveImage();
    void ExitShot();
    void OkShot();
public slots:
//    static ToolBar &Instance();
private:
    QHBoxLayout *mainLayout;
    QToolButton *saveBtn;
    QToolButton *exitBtn;
    QToolButton *okBtn;
};

#endif // SCREENSHOT_H
