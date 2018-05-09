#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "screenshot.h"
#include "stdafx.h"
#include <QMainWindow>
#include <QWebEngineView>
#include <tim_conv_c.h>
namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(const Linkman &linkman, QWidget *parent = 0);
    ~ChatWindow();
    void InitMsgList(bool isLoadedSuccessful);
    void AddContent(QString id, QString nick, time_t time, QString msg);
    void AddFileDesc(const QString &id, const QString &nick, time_t time, const QString &fileName, const QString &filePath, const QString &folderPath);
    void Add2TextEdit(QString msg);
signals:
    void SelectedOrder(int i);
private:
    QString GetMsgHead(const QString &id, const QString &nick, time_t time);
    void GetStyledMsg(const QString &rawMsg, QString *meMsg, QVector<TimMsg> *msgList);
private slots:
    void AddOrderText(int i);
    void on_sendBtn_clicked();
    void on_colorToolButton_clicked(bool checked);
    void on_picToolButton_clicked(bool checked);
    void on_fileToolButton_clicked(bool checked);
    void on_fontToolButton_clicked(bool checked);
    void on_closeBtn_clicked(bool checked);
    void on_actionClose_triggered();
    void on_emotionToolButton_clicked(bool checked);
    void on_shotToolButton_clicked(bool checked);
    void on_clearToolButton_clicked(bool checked);
    void on_recordPushButton_clicked(bool checked);
    void on_orderToolButton_clicked(bool checked);

private:
    Ui::ChatWindow      *ui;
    QWebEngineView      *webView;
    QString             otherId;
    QString             otherNick;
    QString             otherRemark;
    TIMConversationHandle convHandle = nullptr;
    void GetConversation();

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // CHATWINDOW_H
