#ifndef ORDERDIALOG_H
#define ORDERDIALOG_H

#include <QDialog>
//#include "Model/orderlistmodel.h"
#include "Model/ordertablemodel.h"
namespace Ui {
class OrderDialog;
}

class OrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderDialog(QWidget *parent = 0);
    ~OrderDialog();

private:
    Ui::OrderDialog *ui;
    OrderTableModel *model;
};

#endif // ORDERDIALOG_H
