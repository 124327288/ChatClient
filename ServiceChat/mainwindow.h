#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow &Instance();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_myPushButton_clicked();

    void on_addFriendBtn_clicked();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
