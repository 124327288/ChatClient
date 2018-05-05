#ifndef CHATRECORDWINDOW_H
#define CHATRECORDWINDOW_H

#include "webconnect.h"

#include <QMainWindow>
#include <QWebEngineView>

namespace Ui {
class ChatRecordWindow;
}

class ChatRecordWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatRecordWindow(const QString &otherId, const QString &otherNick, QWidget *parent = 0);
    ~ChatRecordWindow();
    void InitMsgList(bool isLoadedSuccessful);
    void AddContent(QString id, QString nick, time_t time, QString msg);
    void AddFileDesc(const QString &id, const QString &nick, time_t time, const QString &fileName, const QString &filePath, const QString &folderPath);
    QString GetMsgHead(const QString &id, const QString &nick, time_t time);
    QWebEngineView *webView() const;
    void setWebView(QWebEngineView *webView);

    QString otherId() const;
    void setOtherId(const QString &otherId);

    QString otherNick() const;
    void setOtherNick(const QString &otherNick);

    WebConnect *webConnect() const;
    void setWebConnect(WebConnect *webConnect);

private:
    Ui::ChatRecordWindow *ui;
    WebConnect          *m_webConnect;
    QWebEngineView      *m_webView;
    QString             m_otherId;
    QString             m_otherNick;
};

#endif // CHATRECORDWINDOW_H
