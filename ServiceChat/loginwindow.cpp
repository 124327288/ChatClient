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
#include "Table/table.h"
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
    ui->loginPushButton->setShortcut(Qt::Key_Return);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    DatabaseTool dbTool = SqliteTool::Database();
    SqliteTool::CreateAll(dbTool);
    dbTool.Select(&idList);
//    SqliteTool::Instance().SelectAll4IdTable(&idList);
    for(auto str : idList)
    {
        qDebug() << str.id();
        ui->usernameComboBox->addItem(str.id());
    }
//    QVector<Account> vec;
//    dbTool.Select(&vec);
//    for(auto acc : vec)
//    {
//        qDebug() << acc.id() << " " << acc.pwd();
//    }
    connect(&TimTool::Instance(), &TimTool::OnLoginSuccess, this, &LoginWindow::onLoginSuccess);
    connect(&TimTool::Instance(), &TimTool::OnLoginError, this, &LoginWindow::onLoginError);
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
    bool isContain = false;
    for(const Id &id : idList)
    {
        if(id.id().toLower() == ui->usernameComboBox->currentText().toLower())
        {
            isContain = true;
            break;
        }
    }
    if(!isContain)
    {
        Id id;
        id.setId(TimTool::Instance().getId());
        SqliteTool::Database().Insert(id);
    }
    emit Signal::Instance().RemPwdAndAutoLogin(ui->rememberCheckBox->isChecked(), ui->autoCheckBox->isChecked());
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
        Account account;
        account.setId(ui->usernameComboBox->currentText());
//        account.setPwd();
//        SqliteTool::Database().Select
        if(SqliteTool::Database().Select<Account>(nullptr, {{"id", ui->usernameComboBox->currentText()}}))
        {
            ui->passwordLineEdit->setText(pwd);
        }
//        if(!SqliteTool::Instance().Select4AccountTable(ui->usernameComboBox->currentText(), &pwd))
//            return;

    }
}
