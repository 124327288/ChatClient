#include "orderdialog.h"
#include "ui_orderdialog.h"

OrderDialog::OrderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderDialog)
{
    ui->setupUi(this);
    model = new OrderTableModel;
    ui->tableView->setModel(model);
}

OrderDialog::~OrderDialog()
{
    delete ui;
}
