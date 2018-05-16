#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include "stdafx.h"
#include <QMainWindow>
#include "Table/table.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT
private:
    explicit LoginWindow(QWidget *parent = 0);
public:
    static LoginWindow &Instance();

    ~LoginWindow();
    void SetLoginLabel(const QString &str);
    void SelfLoginCallback(LOGINRESTYPE state);
signals:
    void RemainTime(int msec);
private slots:
    void on_loginPushButton_clicked();
    void onLoginSuccess();
    void onLoginError(int code, const QString &desc);
    void on_usernameComboBox_currentTextChanged(const QString &arg1);
    void on_registerToolButton_clicked(bool checked);

private:

    Ui::LoginWindow *ui;
    QVector<Id> idList;
};

#endif // LOGINWINDOW_H
