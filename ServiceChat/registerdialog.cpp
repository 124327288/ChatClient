#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "stdafx.h"

#include <QAbstractButton>
#include <QPushButton>
#include "Protocol/C2S/registerprotocol.h"
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    connect(ui->registerPushButton, &QPushButton::clicked, this, &RegisterDialog::onRegister);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::onRegister()
{
    QString user = ui->usernameLineEdit->text();
    QString pwd = ui->passwordLineEdit->text();
    if(user.isEmpty() || pwd.isEmpty())
    {
        POP_ERROR(tr("The user name or password cannot be empty!"));
        return;
    }
    if(user.contains(' ') || pwd.contains(' '))
    {
        POP_ERROR(tr("No Spaces in username or password!"));
        return;
    }
    RegisterProtocol prc;
    prc.setUsername(user);
    prc.setPassword(pwd);
    prc.SendData();
}

void RegisterDialog::on_cancelPushButton_clicked(bool checked)
{
    this->close();
}
