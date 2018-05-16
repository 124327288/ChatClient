#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "stdafx.h"
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();
private slots:
    void onRegisterRet(REGISTERRETTYPE type);
    void onRegister();
    void on_cancelPushButton_clicked(bool checked);

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
