#include "filewindow.h"

#include <Model/filetablemodel.h>

#include <Delegate/filetabledelegate.h>

FileWindow::FileWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
    tableView = new QTableView;
    tableView->setModel(new FileTableModel);
    tableView->setItemDelegate(new FileTableDelegate);
    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(tableView);
    widget->setLayout(mainLayout);
}
