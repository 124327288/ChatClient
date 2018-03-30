#include "filewindow.h"
#include <Model/filetablemodel.h>
#include <Delegate/filetableitemdelegate.h>

FileWindow::FileWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
    tableView = new QTableView;
    tableView->setModel(new FileTableModel);
    tableView->setItemDelegate(new FileTableItemDelegate);
    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(tableView);
    widget->setLayout(mainLayout);
}
