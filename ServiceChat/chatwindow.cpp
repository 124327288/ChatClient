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
    webContent = WebContentHead();
    webView = new QWebEngineView();
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("content", new WebConnect);
    webView->page()->setWebChannel(channel);
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
    webContent += R"123(<h1 onclick="onTest()">请点击该文本</h1>)123";
    webContent += "<br />";
    webView->setHtml(webContent + WebContentTail());
}

void ChatWindow::Add2TextEdit(QString msg)
{
    ui->textEdit->insertHtml(msg);
//    ui->textEdit->append(msg);
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
    DEBUG_VAR(text);
    QRegExp regExp(R"(<p.*>.*</p>)");
    regExp.setMinimal(true);
    QVector<TimMsg> msgList;
    QString beSend;

    auto matchTxtMsg = [](const QString &msg){
        QRegExp rx(R"z(<p.*>.+</p>)z");
        rx.setMinimal(true);
        QStringList txtList;
        for(int pos = 0; (pos = rx.indexIn(msg, pos)) != -1;)
        {
            pos += rx.matchedLength();
            DEBUG_VAR(rx.cap());
            txtList << rx.cap();
        }
        return txtList;
    };

    auto matchImgMsg = [](const QString &msg){
        QRegExp rx(R"z(<img src=\"(?!qrc:)(.*)\" />)z");
        rx.setMinimal(true);
        QStringList imgList;
        for(int pos = 0; (pos = rx.indexIn(msg, pos)) != -1;)
        {
            pos += rx.matchedLength();
            DEBUG_VAR(rx.cap(1));
            imgList << rx.cap(1);
        }
        return imgList;
    };
    for(int pos = 0; (pos = regExp.indexIn(text, pos)) != -1;)
    {
        pos += regExp.matchedLength();
        QString msg = regExp.cap().remove("file:///");
        beSend += msg;
        qDebug() << msg;
        {
            QStringList imgList = matchImgMsg(msg);
            if(imgList.count() > 0)
            {
                qDebug() << "kElemImage";
                for(const auto &s : imgList)
                    msgList += { kElemImage, s };
            }
        }
        msg = msg.remove(QRegularExpression(R"z(<img src=\"(?!qrc:)(.*)\" />)z"));
        {
            QStringList txtList = matchTxtMsg(msg);
            if(txtList.count() > 0)
            {
                qDebug() << "kElemText";
                for(const auto &s : txtList)
                    msgList += { kElemText, s };
            }
        }
    }
    TimTool::Instance().SendMsg(otherId, msgList);
    AddContent(TimTool::Instance().getId(), TimTool::Instance().getNick(), GetTime(), beSend);
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
            ui->textEdit->insertHtml(html);
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
    emotionDialog->move(point.x() - 30, point.y() - 30);
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
    webContent = WebContentHead();
    webView->setHtml("");
    TimTool::Instance().getContentMap().remove(otherId);
}

QString ChatWindow::WebContentHead() const
{
    return R"zzz(
           <!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">
           <html>
           <head>
                <meta name="qrichtext" content="1" />
                <style type="text/css">p, li { white-space: pre-wrap; }</style>
                <script src="./qwebchannel.js"></script>

           </head>
           <body style=" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;" onload="window.scrollTo(0,document.body.scrollHeight); " >
            <script>
                new QWebChannel(qt.webChannelTransport, function(channel) {
                    window.content = channel.objects.content;
                });
                content.SendText.connect(function(msg){alert("Receive Msg: " + msg);});
                function onTest(){
                    if(content){
                        content.ReceiveText("asd");
                    }
                }
            </script>
           )zzz";
}

QString ChatWindow::WebContentTail() const
{
    return R"(
           </body>
           </html>
           )";
}
