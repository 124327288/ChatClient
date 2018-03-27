#include "emotiondialog.h"
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <Delegate/emotionsitemdelegate.h>
#include "stdafx.h"
EmotionDialog::EmotionDialog(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    horCount = 15;
    verCount = 7;
    tableWidget = new QTableWidget;
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QPixmap pixmap(":/emotions/emotions/0fixed.bmp");
    QLabel *label = new QLabel;
    label->setPixmap(pixmap);
    tableWidget->setShowGrid(true);
    tableWidget->setRowCount(verCount);
    tableWidget->setColumnCount(horCount);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->resizeColumnsToContents();
    tableWidget->resizeRowsToContents();
    tableWidget->setItemDelegate(new EmotionsItemDelegate);
    for(int i = 0; i < 99; ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        QString m = QString(":/emotions/emotions/%1fixed.bmp").arg(i);
        item->setData(Qt::DisplayRole, QVariant::fromValue<QPixmap>(QPixmap(m)));
        tableWidget->setItem(i / horCount, i % horCount, item);
    }

    closeBtn = new QPushButton;
    closeBtn->setText(tr("close"));
    bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch(3);
    bottomLayout->addWidget(closeBtn);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);
    resize(400, 230);
    connect(closeBtn, &QPushButton::clicked, this, &EmotionDialog::close);
    connect(tableWidget, &QTableWidget::cellClicked, this, &EmotionDialog::OnCellClicked);
}

void EmotionDialog::OnCellClicked(int row, int column)
{
    int i = row * horCount + column;
    QString imgStr = QString("qrc:/emotions/emotions/%1.gif").arg(i);
    QString html = QString("<img src = %1 />").arg(imgStr);
    chatWindow->Add2TextEdit(html);
    close();
}

ChatWindow *EmotionDialog::getChatWindow() const
{
    return chatWindow;
}

void EmotionDialog::setChatWindow(ChatWindow *value)
{
    chatWindow = value;
}

void EmotionDialog::focusOutEvent(QFocusEvent *event)
{
    DEBUG_FUNC;
    close();
}
