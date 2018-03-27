#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "tim_msg_c.h"
#include "tim_c.h"
#include "Tim/timtool.h"
#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QScrollBar>
#include <QWaitCondition>
#include "View/emotiondialog.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <QFontDialog>
#include <QMessageBox>
#include <ctime>
ChatWindow::ChatWindow(const Linkman &linkman, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->widget->setStyleSheet(QString::fromUtf8("border:1px solid #5CACEE"));
    webContent = R"(<body onload="window.scrollTo(0,document.body.scrollHeight); " >)";
    webView = new QWebEngineView();
//    webView->setStyleSheet(QString::fromUtf8("border:1px solid blue"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(webView);
    ui->widget->setLayout(layout);
//    webView->setProperty("DisplayScrollBars",true);
//    webView->setProperty("DisplayAlerts",false);
//    webView->setStyleSheet("QScrollBar:vertical"
//                           "{"
//                           "width:28px;"
//                           "background:rgba(0,0,0,0%);"
//                           "margin:0px,0px,0px,0px;"
//                           "padding-top:28px;"
//                           "padding-bottom:28px;"
//                           "}"
//                           "QScrollBar::handle:vertical"
//                           "{"
//                           "width:28px;"
//                           "background:rgba(0,0,0,25%);"
//                           " border-radius:4px;"
//                           "min-height:20;"
//                           "}"
//                           "QScrollBar::handle:vertical:hover"
//                           "{"
//                           "width:28px;"
//                           "background:rgba(0,0,0,50%);"
//                           " border-radius:4px;"
//                           "min-height:20;"
//                           "}"
//                           "QScrollBar::add-line:vertical"
//                           "{"
//                           "height:28px;width:28px;"
//                           "subcontrol-position:bottom;"
//                           "}"
//                           "QScrollBar::sub-line:vertical"
//                           "{"
//                           "height:28px;width:28px;"
//                           "subcontrol-position:top;"
//                           "}"
//                           "QScrollBar::add-line:vertical:hover"
//                           "{"
//                           "height:28px;width:28px;"
//                           "subcontrol-position:bottom;"
//                           "}"
//                           "QScrollBar::sub-line:vertical:hover"
//                           "{"
//                           "height:28px;width:28px;"
//                           "subcontrol-position:top;"
//                           "}"
//                           "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
//                           "{"
//                           "background:rgba(0,0,0,10%);"
//                           "border-radius:4px;"
//                           "}"
//                           );
//    webView->setUrl(QUrl("baidu.com"));
    otherId = linkman.id;
    otherNick = !linkman.nick.trimmed().isEmpty() ? linkman.nick : otherId;
    otherRemark = !linkman.remark.trimmed().isEmpty() ? linkman.remark : otherNick;
    TimTool::Instance().AddChatWindowMap(otherId, this);
    setWindowTitle(tr("%1 - Session").arg(otherRemark));
    GetConversation();
    if(!TimTool::Instance().GetContentEX(otherId).isEmpty())
    {
        auto list = TimTool::Instance().GetContentEX(otherId);
        for(auto s : list)
        {
            AddContent(otherId, otherNick, s.time, s.text);
        }
    }
}

ChatWindow::~ChatWindow()
{
    if(ui)
    {
        delete ui;
        ui = nullptr;
    }
}

void ChatWindow::AddContent(QString id, QString nick, time_t time, QString msg)
{
    std::tm *p_tm = std::localtime(&time);
    QString str_time = QString("%1-%2 %3:%4:%5").
            arg(p_tm->tm_mon + 1, 2, 10, QChar('0')).
            arg(p_tm->tm_mday, 2, 10, QChar('0')).
            arg(p_tm->tm_hour, 2, 10, QChar('0')).
            arg(p_tm->tm_min, 2, 10, QChar('0')).
            arg(p_tm->tm_sec, 2, 10, QChar('0'));
    QString title = QString(R"(
                              <font color="blue">%1(%2) %3</font>
                              )").arg(id).arg(nick).arg(str_time);
//    ui->textBrowser->append(title);
//    ui->textBrowser->append(msg);
//    ui->textBrowser->append("");
    webContent += title;
//    webContent += R"(<img src =qrc:/emotions/emotions/0.gif />)";
    webContent += "<br />";
    webContent += msg;
    webContent += "<br />";

    webView->setHtml(webContent);
//    webView->update();
//    if(auto v = webView->)

//    if(auto verticalScrollBar = ui->textBrowser->verticalScrollBar())
//    {
//        verticalScrollBar->setSliderPosition(verticalScrollBar->maximum());
//    }

//    if(auto horizontalScrollBar = ui->textBrowser->horizontalScrollBar())
//    {
//        horizontalScrollBar->setSliderPosition(horizontalScrollBar->minimum());
//    }
}

void ChatWindow::Add2TextEdit(QString text)
{
    ui->textEdit->append(text);
//    webContent += text;
//    webView->setHtml(webContent);
}

void ChatWindow::GetConversation()
{
    if(TimTool::Instance().ContainInConvMap(otherId))
    {
        convHandle = TimTool::Instance().GetConvHandle(otherId);
        return;
    }
    QByteArray bytes = otherId.toUtf8();
    convHandle = CreateConversation();
	if (!ON_INVOKE(TIMGetConversation, convHandle, kCnvC2C, bytes.data()))
	{
		TimTool::Instance().AddConvMap(otherId, convHandle);
		emit TimTool::Instance().NewConversation(otherId, otherNick, GetTime());
	}
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    TimTool::Instance().RemoveChatWindowMap(otherId);
    TimTool::Instance().RemoveConvMap(otherId);
    DestroyConversation(convHandle);
}

void ChatWindow::on_sendBtn_clicked()
{
    if(ui->textEdit->toPlainText().isEmpty())
        return;
    QString text = ui->textEdit->toHtml();
    qDebug() << text;
    TimTool::Instance().SendMsg(otherId, text);
    AddContent(TimTool::Instance().getId(), TimTool::Instance().getNick(), GetTime(), text);
    ui->textEdit->clear();
}

//void ChatWindow::on_fontComboBox_currentFontChanged(const QFont &f)
//{
//    ui->textEdit->setCurrentFont(f);
//    ui->textEdit->setFocus();
//}

//void ChatWindow::on_comboBox_currentIndexChanged(const QString &arg1)
//{
//    ui->textEdit->setFontPointSize(arg1.toDouble());
//    ui->textEdit->setFocus();
//}

//void ChatWindow::on_boldToolButton_clicked(bool checked)
//{
//    if(checked)
//    {
//        ui->textEdit->setFontWeight(QFont::Bold);
//    }
//    else
//    {
//        ui->textEdit->setFontWeight(QFont::Normal);
//    }
//    ui->textEdit->setFocus();
//}

//void ChatWindow::on_italicToolButton_clicked(bool checked)
//{
//    ui->textEdit->setFontItalic(checked);
//    ui->textEdit->setFocus();
//}

//void ChatWindow::on_lineToolButton_clicked(bool checked)
//{
//    ui->textEdit->setFontUnderline(checked);
//    ui->textEdit->setFocus();
//}

void ChatWindow::on_colorToolButton_clicked(bool checked)
{
    auto color = QColorDialog::getColor(ui->textEdit->textColor(), this);
    if(color.isValid())
    {
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFocus();
    }
}

void ChatWindow::on_picToolButton_clicked(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Images (*.jpg *.xpm *.png);;"));
    if(!fileName.isNull())
    {
        if(!QMessageBox::information(this, tr("Send this Image?"), tr("Send this Image?"), tr("Ok"), tr("Cancel")))
        {
            QString html = QString(R"(<img src = "%1" />)").arg(fileName);
            AddContent(TimTool::Instance().getId(), TimTool::Instance().getNick(), GetTime(), html);
            TimTool::Instance().SendImage(otherId, fileName);
    //        ui->textEdit->append(html);
        }
    }
}

void ChatWindow::on_fileToolButton_clicked(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString());
    if(!fileName.isNull())
    {
        if(!QMessageBox::information(this, tr("Send this File?"), tr("Send this File?"), tr("Ok"), tr("Cancel")))
        {
            QString html = tr("Send File: %1").arg(fileName);
            AddContent(TimTool::Instance().getId(), TimTool::Instance().getNick(), GetTime(), html);
            TimTool::Instance().SendFile(otherId, fileName);
        }
    }
}

void ChatWindow::on_fontToolButton_clicked(bool checked)
{
    QFont lastFont = ui->textEdit->toPlainText().isEmpty() ? QFont("宋体") : ui->textEdit->currentFont();
    QFont font = QFontDialog::getFont(nullptr,lastFont , this);
    ui->textEdit->setCurrentFont(font);
    ui->textEdit->setFocus();
}

void ChatWindow::on_closeBtn_clicked(bool checked)
{
    close();
}

void ChatWindow::on_actionClose_triggered()
{
    close();
}

void ChatWindow::on_emotionToolButton_clicked(bool checked)
{
//    DEBUG_FUNC;
    auto emotionDialog = new EmotionDialog;
    emotionDialog->setChatWindow(this);
    emotionDialog->setGeometry(geometry().left() + ui->emotionToolButton->geometry().left(),
                               geometry().top() + ui->emotionToolButton->geometry().bottom() + 30,
                               emotionDialog->size().width(),
                               emotionDialog->size().height());
    emotionDialog->show();
}
