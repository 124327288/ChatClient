#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include "stdafx.h"
#include <QMainWindow>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    static LoginWindow &Instance();

    ~LoginWindow();
    void SetLoginLabel(const QString &str);
signals:
    void RemainTime(int msec);
private slots:

    void on_loginPushButton_clicked();
    void onLoginSuccess();
    void onLoginError(int code, const QString &desc);
private:
    explicit LoginWindow(QWidget *parent = 0);
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
