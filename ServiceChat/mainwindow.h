#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "stdafx.h"
#include <QListView>
#include <QMainWindow>
#include "Model/friendlistmodel.h"
#include "Model/sessionlistmodel.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    explicit MainWindow(QWidget *parent = 0);
    void InitUI();
public:
    static MainWindow &Instance();
    ~MainWindow();
private slots:
    void on_addFriendBtn_clicked();

    void updateListView();

private:
    Ui::MainWindow  *ui;
    QListView       *sessionListView;
    QListView       *friendListView;
    FriendListModel *friendListModel;
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
