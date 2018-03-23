#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "Tim/timtool.h"
#include "luatool.h"
#include "mainwindow.h"
#include "signal.h"
#include "sqlitetool.h"
#include <QDebug>
#include <QMessageBox>
#include <QTranslator>
LoginWindow &LoginWindow::Instance()
{
    static LoginWindow instance;
    return instance;
}

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    SqliteTool::Instance().SelectAll4IdTable(&idList);
    for(auto str : idList)
        qDebug() << str;
    ui->usernameComboBox->addItems(idList);
    ui->autoCheckBox->setChecked(LuaTool::Instance().getAutoLogin());
    ui->rememberCheckBox->setChecked(LuaTool::Instance().getRememberPassword());
    connect(&TimTool::Instance(), &TimTool::LoginSuccess, this, &LoginWindow::onLoginSuccess);
    connect(&TimTool::Instance(), &TimTool::LoginError, this, &LoginWindow::onLoginError);
    connect(this, &LoginWindow::RemainTime, [=](int msec){
        QString s;
        if(msec > 0)
        {
            s = tr("Logining(%1)").arg(QString::number(msec/1000., 'g', 2));
        }
        else
        {
            s = tr("Login timeout!Click retry.");
        }
        SetLoginLabel(s);
    });
    connect(ui->autoCheckBox, &QCheckBox::toggled, ui->rememberCheckBox, &QCheckBox::setChecked);
    if(ui->autoCheckBox->isChecked())
    {
        QString sig;
//        SqliteTool::Instance().Select4SignTable()
    }
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::SetLoginLabel(const QString &str)
{
    ui->loginPushButton->setText(str);
}

void LoginWindow::on_loginPushButton_clicked()
{
    QString username = ui->usernameComboBox->currentText();
    QString password = ui->passwordLineEdit->text();
    if(username.trimmed().isEmpty())
    {
        QMessageBox::critical(this, tr("ERROR"), tr("Username cannot be empty!"));
        return;
    }
    else if(password.trimmed().isEmpty())
    {
        QMessageBox::critical(this, tr("ERROR"), tr("Password cannot be empty!"));
        return;
    }
    SetLoginLabel(tr("Logining......"));

    TimTool::Instance().Login(username, password);

}

void LoginWindow::onLoginSuccess()
{
    close();
    DEBUG_FUNC;
    DEBUG_VAR(ui->usernameComboBox->currentText());
    if(!idList.contains(ui->usernameComboBox->currentText(), Qt::CaseInsensitive))
        SqliteTool::Instance().Insert2IdTable(TimTool::Instance().getId());
    emit Signal::Instance().RemPwdAndAutoLogin(ui->rememberCheckBox->isChecked(), ui->autoCheckBox->isChecked());
//    emit Signal::Instance().SetRememberPassword(ui->rememberCheckBox->isChecked());
//    emit Signal::Instance().SetAutoLogin(ui->autoCheckBox->isChecked());
    TimTool::Instance().GetFriendList();
    MainWindow::Instance().show();
}

void LoginWindow::onLoginError(int code, const QString &desc)
{
    qDebug() << QString("Error! code = %1 desc = %2").arg(code).arg(desc);
    QMessageBox::critical(this, tr("ERROR"), QString("Error! code = %1 desc = %2").arg(code).arg(desc));
    SetLoginLabel(tr("Login"));
}
