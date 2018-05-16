#include "orderdialog.h"
#include "ui_orderdialog.h"
#include "chatwindow.h"
OrderDialog::OrderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("My Order"));
    model = new OrderTableModel;
    ui->tableView->setModel(model);
    connect(ui->tableView, &QTableView::clicked, this, &OrderDialog::SendOrder);
}

OrderDialog::~OrderDialog()
{
    delete ui;
}

void OrderDialog::SendOrder(const QModelIndex &index)
{
    int r = index.row();
    DEBUG_VAR(r);
    ChatWindow *window = dynamic_cast<ChatWindow*>(parent());
    if(window)
    {
        emit window->SelectedOrder(r);
        close();
    }
}
