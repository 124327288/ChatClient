#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "stdafx.h"
#include <QListView>
#include <QMainWindow>
#include <QWebEngineView>
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
	void SetNickName(const QString &nick);
	void PopChatWindow(const QModelIndex &index);
public:
    static MainWindow &Instance();
    ~MainWindow();
private slots:
    void popOrderDialog();
    void popSettingDialog();
    void updateListView();
    void on_addFriendBtn_clicked();    
    void on_actionLogout_triggered(bool checked);
    void on_actionExit_triggered(bool checked);
    void on_actionAdd_a_friend_triggered(bool checked);
    void on_actionSession_with_triggered(bool checked);

private:
    Ui::MainWindow      *ui;
    QListView           *sessionListView;
    SessionListModel    *sessionListModel;
    QListView           *friendListView;
    FriendListModel     *friendListModel;
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
