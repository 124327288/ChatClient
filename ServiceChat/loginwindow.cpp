#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "Tim/timtool.h"
#include "mainwindow.h"
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
    connect(ui->rememberCheckBox, &QCheckBox::clicked, [=](bool checked){
       if(checked)
       {
           QTranslator trans;
           trans.load(":/res/client_cn");

       }
    });
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
    TimTool::Instance().GetFriendList();
    MainWindow::Instance().show();
}

void LoginWindow::onLoginError(int code, const QString &desc)
{
    qDebug() << QString("Error! code = %1 desc = %2").arg(code).arg(desc);
    QMessageBox::critical(this, tr("ERROR"), QString("Error! code = %1 desc = %2").arg(code).arg(desc));
    SetLoginLabel(tr("Login"));
}
