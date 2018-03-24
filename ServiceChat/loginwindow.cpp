#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "Tim/timtool.h"
#include "luatool.h"
#include "mainwindow.h"
#include "signal.h"
#include "sqlitetool.h"
#include <string>
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
    connect(&TimTool::Instance(), &TimTool::LoginSuccess, this, &LoginWindow::onLoginSuccess);
    connect(&TimTool::Instance(), &TimTool::LoginError, this, &LoginWindow::onLoginError);
//    connect(ui->usernameComboBox, &QComboBox::currentIndexChanged,)
//    connect(this, &LoginWindow::RemainTime, [=](int msec){
//        QString s;
//        if(msec > 0)
//        {
//            s = tr("Logining(%1)").arg(QString::number(msec/1000., 'g', 2));
//        }
//        else
//        {
//            s = tr("Login timeout!Click retry.");
//        }
//        SetLoginLabel(s);
//    });
    connect(ui->autoCheckBox, &QCheckBox::toggled, ui->rememberCheckBox, &QCheckBox::setChecked);
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
    TimTool::Instance().GetFriendList();
    MainWindow::Instance().show();
}

void LoginWindow::onLoginError(int code, const QString &desc)
{
    qDebug() << QString("Error! code = %1 desc = %2").arg(code).arg(desc);
    QMessageBox::critical(this, tr("ERROR"), QString("Error! code = %1 desc = %2").arg(code).arg(desc));
    SetLoginLabel(tr("Login"));
}

void LoginWindow::on_usernameComboBox_currentTextChanged(const QString &arg1)
{
    std::string userDir = LuaTool::Instance().makeUserDirString(arg1);
    if(!LuaTool::Instance().isUserDirExist(userDir) || !LuaTool::Instance().isUserCfgFileExist(userDir) )
        return;
    UserCfgStruct cfg;
    LuaTool::Instance().getUserConfig(userDir, &cfg);
    /* 需要根据选择的用户id判断*/
    ui->autoCheckBox->setChecked(cfg.autoLogin);
    ui->rememberCheckBox->setChecked(cfg.rememberPassword);
    if(ui->autoCheckBox->isChecked())
    {
//        QString sig;
//        SqliteTool::Instance().Select4SignTable()
    }
    if(ui->rememberCheckBox->isChecked())
    {
        QString pwd;
        if(!SqliteTool::Instance().Select4AccountTable(ui->usernameComboBox->currentText(), &pwd))
            return;
        ui->passwordLineEdit->setText(pwd);
    }
}
