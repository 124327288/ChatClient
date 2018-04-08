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
#include <QWebChannel>
#include "emotiondialog.h"
#include "screenshot.h"
#include <Windows.h>
#include <QFontDialog>
#include <QLabel>
#include <QMessageBox>
#include <QWebEngineSettings>
#include <ctime>
#include "webconnect.h"
#include <tuple>
#include <Tim/chatmanager.h>
ChatWindow::ChatWindow(const Linkman &linkman, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->sendBtn->setShortcut(QString("Ctrl+Return"));
    ui->widget->setStyleSheet(QString::fromUtf8("border:1px solid #5CACEE"));
    ui->textEdit->installEventFilter(this);
    QWebChannel *channel = new QWebChannel;
    channel->registerObject("connect", &WebConnect::Instance());
    webView = new QWebEngineView();
    webView->page()->setWebChannel(channel);
    webView->load(QString("file:///%1/%2").arg(QDir::currentPath()).arg("ChatView/index.html"));
    connect(webView, &QWebEngineView::loadFinished, this, &ChatWindow::InitMsgList);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(webView);
    ui->widget->setLayout(layout);
    otherId = linkman.id;
    otherNick = !linkman.nick.trimmed().isEmpty() ? linkman.nick : otherId;
//    otherRemark = !linkman.remark.trimmed().isEmpty() ? linkman.remark : otherNick;
    TimTool::Instance().AddChatWindowMap(otherId, this);
    setWindowTitle(tr("%1 - Session").arg(otherNick));
    GetConversation();
//    if(!TimTool::Instance().GetContentEX(otherId).isEmpty())
//    {
//        auto list = TimTool::Instance().GetContentEX(otherId);
//        for(auto s : list)
//        {
//            AddContent(otherId, otherNick, s.time, s.text);
//        }
//    }
}

ChatWindow::~ChatWindow()
{
    if(ui)
    {
        delete ui;
        ui = nullptr;
    }
    webView->deleteLater();
}

void ChatWindow::InitMsgList(bool isLoadedSuccessful)
{
    DEBUG_VAR(isLoadedSuccessful);
    if(!isLoadedSuccessful)
        return;
    QTimer::singleShot(200, this, [=]{
        auto &chatMap = ChatManager::Instance().getChatMap();
        auto msgList = chatMap[otherId];
        DEBUG_VAR(msgList.count());
        for(auto msg: msgList)
        {
            DEBUG_VAR(msg.text);
            auto id = msg.isMine ? TimTool::Instance().getId() : otherId;
            auto nick = msg.isMine ? TimTool::Instance().getNick() : otherNick;
            AddContent(id, nick, msg.time, msg.text);
        }
    });
}

void ChatWindow::AddContent(QString id, QString nick, time_t time, QString msg)
{
    DEBUG_FUNC;
    QString title = GetMsgHead(id, nick, time);
    emit WebConnect::Instance().AddContent(title, msg);
}

void ChatWindow::AddFileDesc(const QString &id, const QString &nick, time_t time, const QString &fileName, const QString &filePath, const QString &folderPath)
{
    DEBUG_FUNC;
    QString title = GetMsgHead(id, nick, time);
    emit WebConnect::Instance().AddFileDesc(title, fileName, filePath, folderPath);
}

void ChatWindow::Add2TextEdit(QString msg)
{
    ui->textEdit->insertHtml(msg);
}

QString ChatWindow::GetMsgHead(const QString &id, const QString &nick, time_t time)
{
    std::tm *p_tm = std::localtime(&time);
    QString str_time = QString("%1-%2 %3:%4:%5").
            arg(p_tm->tm_mon + 1, 2, 10, QChar('0')).
            arg(p_tm->tm_mday, 2, 10, QChar('0')).
            arg(p_tm->tm_hour, 2, 10, QChar('0')).
            arg(p_tm->tm_min, 2, 10, QChar('0')).
            arg(p_tm->tm_sec, 2, 10, QChar('0'));
    QString title = QString("%1(%2) %3").arg(id).arg(nick).arg(str_time);
    return title;
}

void ChatWindow::GetStyledMsg(const QString &rawMsg, QString *meMsg, QVector<TimMsg> *msgList)
{
    if(!meMsg || !msgList)
        return;
    auto matchTxtMsg = [](const QString &msg){
        QRegExp rx(R"z(<p.*>.+</p>)z");
        rx.setMinimal(true);
        QVector<std::tuple<QString, int>> txtList;
        for(int pos = 0; (pos = rx.indexIn(msg, pos)) != -1;)
        {
            pos += rx.matchedLength();
//            DEBUG_VAR(rx.cap());
//            txtList << rx.cap();
            txtList += { rx.cap(), pos };
        }
        return txtList;
    };

    auto matchImgMsg = [](const QString &msg){
        QRegExp rx(R"z(<img src=\"(?!qrc:)(.*)\" />)z");
        rx.setMinimal(true);
        QVector<std::tuple<QString, int>> imgList;
        for(int pos = 0; (pos = rx.indexIn(msg, pos)) != -1;)
        {
            pos += rx.matchedLength();
//            DEBUG_VAR(rx.cap(1));
//            imgList << rx.cap(1);
            imgList += { rx.cap(1), pos };
        }
        return imgList;
    };
    QRegExp regExp(R"(<p.*>.*</p>)");
    regExp.setMinimal(true);
//    QString txtMsg;
    QVector<std::tuple<TimMsg, int>> _msgList;
    for(int pos = 0; (pos = regExp.indexIn(rawMsg, pos)) != -1;)
    {
        pos += regExp.matchedLength();
        QString msg = regExp.cap().remove("file:///");
        *meMsg += msg;
        qDebug() << msg;
        {
            auto imgList = matchImgMsg(msg);
            if(imgList.count() > 0)
            {
                qDebug() << "kElemImage";
                for(const auto &s : imgList)
                {
                    _msgList += { TimMsg{ kElemImage, std::get<0>(s)} , std::get<1>(s) };
                }
            }
        }
        msg = msg.remove(QRegularExpression(R"z(<img src=\"(?!qrc:)(.*)\" />)z"));
        {
            auto txtList = matchTxtMsg(msg);
            if(txtList.count() > 0)
            {
                qDebug() << "kElemText";
                for(const auto &s : txtList)
                    _msgList += { TimMsg{ kElemText, std::get<0>(s)} , std::get<1>(s) };
//                    txtMsg += s;
//                    *msgList += { kElemText, s };
            }
        }
    }
    qSort(_msgList.begin(), _msgList.end(), [](const std::tuple<TimMsg, int> &a, const std::tuple<TimMsg, int> &b){
        return std::get<1>(a) < std::get<1>(b);
    });
    for(const std::tuple<TimMsg, int> &t: _msgList)
    {
        int type = std::get<0>(t).type;
        if(type == kElemImage)
            *msgList += std::get<0>(t);
        else if(type == kElemText)
        {
            if(msgList->back().type == type)
            {
                msgList->back().msg += std::get<0>(t).msg;
            }
            else
            {
                *msgList += std::get<0>(t);
            }
        }
    }

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
    if(convHandle)
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
                    QString path = QDir::currentPath() + GetCacheDirName() + UuidToStringEx() + ".png";
                    img.save(path);
                    ui->textEdit->insertHtml(QString("<img src = %1 />").arg(path));
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
    QString meMsg;
    QVector<TimMsg> msgList;
    GetStyledMsg(text, &meMsg, &msgList);
    DEBUG_VAR(meMsg);
    TimTool::Instance().SendMsg(otherId, msgList);
    ChatManager::Instance().AddToChatMap(otherId, { true, GetTime(), meMsg });
    AddContent(TimTool::Instance().getId(), TimTool::Instance().getNick(), GetTime(), meMsg);
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
        QString html = QString(R"(<img src = "%1" />)").arg(fileName);
        ui->textEdit->insertHtml(html);
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
    emotionDialog->move(point.x() - 30, point.y() - 30);
    emotionDialog->show();
}

void ChatWindow::on_shotToolButton_clicked(bool checked)
{
    ScreenShot *shot = new ScreenShot();
    shot->setChatWindow(this);
    shot->show();
}

void ChatWindow::on_clearToolButton_clicked(bool checked)
{
    webView->reload();
//    TimTool::Instance().getContentMap().remove(otherId);
}
