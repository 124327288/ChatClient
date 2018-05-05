#include "chatrecordwindow.h"
#include "ui_chatrecordwindow.h"
#include "webconnect.h"
#include "Tim/chatmanager.h"
#include "Tim/timtool.h"
#include <QBoxLayout>
#include <QWebChannel>

ChatRecordWindow::ChatRecordWindow(const QString &otherId, const QString &otherNick, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatRecordWindow)
{
    ui->setupUi(this);
    setOtherId(otherId);
    setOtherNick(otherNick);
    if(m_otherNick.isEmpty())
        setOtherNick(otherId);
    setWindowTitle(tr("Chatting Records With %1").arg(m_otherNick));
    m_webConnect = new WebConnect(this);
    QWebChannel *channel = new QWebChannel;
    channel->registerObject("connect", m_webConnect);
    m_webView = new QWebEngineView();
    connect(m_webView, &QWebEngineView::loadFinished, this, &ChatRecordWindow::InitMsgList);
    m_webView->page()->setWebChannel(channel);
    m_webView->load(QString("file:///%1/%2").arg(QDir::currentPath()).arg("ChatView/index.html"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_webView);
    ui->widget->setLayout(layout);
}

ChatRecordWindow::~ChatRecordWindow()
{
    delete ui;
}

void ChatRecordWindow::InitMsgList(bool isLoadedSuccessful)
{
    if(!isLoadedSuccessful)
        return;
    QTimer::singleShot(200, this, [=]{
        auto &chatMap = ChatManager::Instance().getChatMap();
        auto msgList = chatMap[otherId()];
        DEBUG_VAR(msgList.count());
        for(auto msg: msgList)
        {
            DEBUG_VAR(msg.text);
            auto id = msg.isMine ? TimTool::Instance().getId() : otherId();
            auto nick = msg.isMine ? TimTool::Instance().getNick() : otherNick();
            AddContent(id, nick, msg.time, msg.text);
        }
    });
}

void ChatRecordWindow::AddContent(QString id, QString nick, time_t time, QString msg)
{
    DEBUG_FUNC;
    QString title = GetMsgHead(id, nick, time);
    emit m_webConnect->AddContent(title, msg);
}

void ChatRecordWindow::AddFileDesc(const QString &id, const QString &nick, time_t time, const QString &fileName, const QString &filePath, const QString &folderPath)
{
    DEBUG_FUNC;
    QString title = GetMsgHead(id, nick, time);
    emit m_webConnect->AddFileDesc(title, fileName, filePath, folderPath);
}

QString ChatRecordWindow::GetMsgHead(const QString &id, const QString &nick, time_t time)
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

QWebEngineView *ChatRecordWindow::webView() const
{
    return m_webView;
}

void ChatRecordWindow::setWebView(QWebEngineView *webView)
{
    m_webView = webView;
}

QString ChatRecordWindow::otherId() const
{
    return m_otherId;
}

void ChatRecordWindow::setOtherId(const QString &otherId)
{
    m_otherId = otherId;
}

QString ChatRecordWindow::otherNick() const
{
    return m_otherNick;
}

void ChatRecordWindow::setOtherNick(const QString &otherNick)
{
    m_otherNick = otherNick;
}

WebConnect *ChatRecordWindow::webConnect() const
{
    return m_webConnect;
}

void ChatRecordWindow::setWebConnect(WebConnect *webConnect)
{
    m_webConnect = webConnect;
}
