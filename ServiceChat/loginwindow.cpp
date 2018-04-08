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
using namespace std;
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
    sort(idList.begin(), idList.end(), [](const auto &obj1, const auto &obj2){
        return obj1.time() > obj2.time();
    });
    for_each(idList.begin(), idList.end(), [=](const auto &id){
//        qDebug() << id.id() << id.time();
        ui->usernameComboBox->addItem(id.id());
    });
    connect(&TimTool::Instance(), &TimTool::OnLoginSuccess, this, &LoginWindow::onLoginSuccess);
    connect(&TimTool::Instance(), &TimTool::OnLoginError, this, &LoginWindow::onLoginError);
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
    bool isContain = find_if(idList.begin(), idList.end(), [=](const Id &id){
        return id.id().toLower() == ui->usernameComboBox->currentText().toLower();
        }) != idList.end();
    BEGIN
        Id id;
        id.setId(TimTool::Instance().getId());
        id.setTime(GetTime());
        DatabaseTool dbTool = SqliteTool::Database();
        if(isContain)
            DEBUG_VAR(dbTool.Update(id, {{"time", id.time()}}, {{"id", id.id()}}));
        else
            DEBUG_VAR(dbTool.Insert(id));
    END

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
        QVector<Account> accountList;
        if(SqliteTool::Database().Select(&accountList, {{"id", ui->usernameComboBox->currentText()}}))
        {
            ui->passwordLineEdit->setText(accountList[0].pwd());
        }

    }
}
