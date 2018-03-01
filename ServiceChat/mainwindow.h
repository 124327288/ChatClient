#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "exptype.h"
#include "listmodel.h"
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
    void on_addFriendBtn_clicked();

    void updateListView();

private:
    Ui::MainWindow *ui;
    ListModel *listModel;
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
