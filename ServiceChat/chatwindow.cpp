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
#include "screenshot.h"
#include <Windows.h>
#include <QFontDialog>
#include <QLabel>
#include <QMessageBox>
#include <QWebEngineSettings>
#include <ctime>
ChatWindow::ChatWindow(const Linkman &linkman, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->sendBtn->setShortcut(QString("Ctrl+Return"));
    ui->widget->setStyleSheet(QString::fromUtf8("border:1px solid #5CACEE"));
    ui->textEdit->installEventFilter(this);
//    screenShot = new ScreenShot(this);
//    scree
    webContent = R"(<body onload="window.scrollTo(0,document.body.scrollHeight); " >)";
    webView = new QWebEngineView();
//    webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
//    webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
//    webView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
//    webView->setStyleSheet(QString::fromUtf8("border:1px solid blue"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(webView);
    ui->widget->setLayout(layout);
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
//    connect()
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
    webContent += title;
    webContent += "<br />";
    webContent += msg;
    webContent += "<br />";
    webView->setHtml(webContent);
}

void ChatWindow::Add2TextEdit(QString msg)
{
    ui->textEdit->append(msg);
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

bool ChatWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_V)
            {
                QClipboard *board = QApplication::clipboard();
                if(board->mimeData()->hasImage())
                {
                    QImage img = board->image();
                    QString path = QDir::currentPath() + GetCacheDirName() + QUuid::createUuid().toString() + ".png";
                    img.save(path);
                    ui->textEdit->append(QString("<img src = %1 />").arg(path));
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void ChatWindow::on_sendBtn_clicked()
{
    if(ui->textEdit->toPlainText().trimmed().isEmpty())
        return;
    QString text = ui->textEdit->toHtml();
    QRegExp regExp(R"(<p.*>.*</p>)");
    regExp.setMinimal(true);
    QVector<TimMsg> msgList;
    auto isImgMsg = [](const QString &msg){
        // <img src=\"qrc:/emotions/emotions/3.gif\" /> no solve
        QRegExp rx(R"z(<img src=\"(.*)\" />)z");
        if(rx.indexIn(msg) != -1)
        {
            return rx.cap(1);
        }
        return QString();
    };
    for(int pos = 0; (pos = regExp.indexIn(text, pos)) != -1;)
    {
        pos += regExp.matchedLength();
        QString msg = regExp.cap().remove("file:///");
        DEBUG_VAR(msg);
        QString imgMsg = isImgMsg(msg);
        if(imgMsg.isNull())
        {
            msgList += { kElemText, msg };
        }
        else
        {
            msgList += { kElemImage, imgMsg };
        }
    }
    TimTool::Instance().SendMsg(otherId, msgList);
    AddContent(TimTool::Instance().getId(), TimTool::Instance().getNick(), GetTime(), text);
    ui->textEdit->clear();
}

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
//        if(!QMessageBox::information(this, tr("Send this Image?"), tr("Send this Image?"), tr("Ok"), tr("Cancel")))
        {
            QString html = QString(R"(<img src = "%1" />)").arg(fileName);
//            AddContent(TimTool::Instance().getId(), TimTool::Instance().getNick(), GetTime(), html);
//            TimTool::Instance().SendImage(otherId, fileName);
            ui->textEdit->append(html);
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
    QPoint point = QCursor::pos();
    auto emotionDialog = new EmotionDialog;
    emotionDialog->setChatWindow(this);
    emotionDialog->move(point.x(), point.y());
    emotionDialog->show();
}

void ChatWindow::on_shotToolButton_clicked(bool checked)
{
//    ScreenShot::BeginShot();
    ScreenShot *shot = new ScreenShot();
    shot->setChatWindow(this);
    shot->show();
}

void ChatWindow::on_clearToolButton_clicked(bool checked)
{
    webView->setHtml("");
}
