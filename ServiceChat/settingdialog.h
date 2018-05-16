#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    SettingDialog(QWidget *parent = nullptr);

signals:

public slots:

private:
    //left
    QListWidget *list;
    //right
    QStackedWidget *stackWidget;
    //pageUser
    QWidget *userWidget;
    QGridLayout *gridLayout;
    QLabel *idLabel;
    QLineEdit *idLine;
    QLabel *nickLabel;
    QLineEdit *nickLine;
    QLabel *sendMsgLabel;
    QComboBox *sendMsgComboBox;

    //pageSystem
    QWidget *systemWidget;
    QLabel *langLabel;
    QComboBox *langCombo;
    int langComboCurIndex;
    QHBoxLayout *hLayout2;

    QPushButton *btnOk;
    QPushButton *btnCancel;
    QHBoxLayout *btnLayout;

    QGridLayout *mainLayout;
};

#endif // SETTINGDIALOG_H
