#include "settingdialog.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <luatool.h>
#include "Tim/timtool.h"
SettingDialog::SettingDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Settings"));
    list = new QListWidget(this);
    list->addItem(tr("User Settings"));
    list->addItem(tr("System Settings"));
    stackWidget = new QStackedWidget;
    stackWidget->setFrameStyle(QFrame::Panel|QFrame::Raised);
    //userPage
    userWidget = new QWidget;
    idLabel = new QLabel("ID");
    idLine = new QLineEdit;
    idLine->setText(TimTool::Instance().getId());
    idLine->setEnabled(false);
    nickLabel = new QLabel(tr("nick"));
    nickLine = new QLineEdit;
    nickLine->setText(TimTool::Instance().getNick());
    gridLayout = new QGridLayout;
    gridLayout->addWidget(idLabel, 0, 0);
    gridLayout->addWidget(idLine, 0, 1);
    gridLayout->addWidget(nickLabel, 1, 0);
    gridLayout->addWidget(nickLine, 1, 1);
    gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    userWidget->setLayout(gridLayout);
    //systemPage
    systemWidget = new QWidget;
    langLabel = new QLabel(tr("Language"));
    langCombo = new QComboBox;
    langCombo->addItems({"English", "Simplified Chinese"});
    langCombo->setCurrentIndex(LuaTool::Instance().getLanguage() == "en" ? 0 : 1);
    langComboCurIndex = langCombo->currentIndex();
    hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(langLabel);
    hLayout2->addWidget(langCombo);
    hLayout2->setSizeConstraint(QLayout::SetFixedSize);
    systemWidget->setLayout(hLayout2);
    stackWidget->addWidget(userWidget);
    stackWidget->addWidget(systemWidget);

    btnOk = new QPushButton(tr("Ok"));
    btnCancel = new QPushButton(tr("Cancel"));
    btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);


    mainLayout = new QGridLayout(this);
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(list, 0, 0);
    mainLayout->addWidget(stackWidget, 0, 1);
    mainLayout->addLayout(btnLayout, 1, 0, 1, 2);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 2);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    connect(list, &QListWidget::currentRowChanged, stackWidget, &QStackedWidget::setCurrentIndex);
    connect(btnCancel, &QPushButton::clicked, [=]{close();});
    connect(btnOk, &QPushButton::clicked, [=]{
        switch (stackWidget->currentIndex())
        {
            case 0:
            {
//                qDebug() << "nickLabel->text(): " << nickLine->text();
                TimTool::Instance().SetNickName(nickLine->text());
                break;
            }
            case 1:
            {
                int i = langCombo->currentIndex();
                if(i != langComboCurIndex)
                {
                    langComboCurIndex = i;
                    if(langComboCurIndex == 0)
                        LuaTool::Instance().setLanguage("en");
                    else if(langComboCurIndex == 1)
                        LuaTool::Instance().setLanguage("cn");
                    else
                        break;
                    LuaTool::Instance().updateAppCfgFile();
                }
                break;
            }
        }
        close();
    });
}
