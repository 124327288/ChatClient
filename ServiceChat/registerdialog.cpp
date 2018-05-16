#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "stdafx.h"

#include <QAbstractButton>
#include <QPushButton>
#include "Protocol/C2S/registerprotocol.h"
#include "signal.h"
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    connect(ui->registerPushButton, &QPushButton::clicked, this, &RegisterDialog::onRegister);
    connect(&Signal::Instance(), &Signal::RegisterRet, this, &RegisterDialog::onRegisterRet);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::onRegisterRet(REGISTERRETTYPE type)
{
    switch (type)
    {
    case REGISTERRETTYPE::SUCCESS:
        POP_INFOMATION(tr("Register Success!"));
        close();
        break;
    case REGISTERRETTYPE::EXIST:
        POP_ERROR(tr("The username already exists."));
        break;
    default:
        break;
    }
}

void RegisterDialog::onRegister()
{
    QString user = ui->usernameLineEdit->text();
    QString pwd = ui->passwordLineEdit->text();
    if(user.isEmpty() || pwd.isEmpty())
    {
        POP_ERROR(tr("The username or password cannot be empty!"));
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
