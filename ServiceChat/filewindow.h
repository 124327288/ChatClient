#ifndef FILEWINDOW_H
#define FILEWINDOW_H

#include <QBoxLayout>
#include <QMainWindow>
#include <QTableView>

class FileWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FileWindow(QWidget *parent = nullptr);

signals:

public slots:
private:
    QHBoxLayout *mainLayout;
    QTableView *tableView;
};

#endif // FILEWINDOW_H
