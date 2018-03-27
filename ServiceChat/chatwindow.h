#ifndef CHATWINDOW_H
#define CHATWINDOW_H

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
    void AddContent(QString id, QString nick, time_t time, QString msg);
    void Add2TextEdit(QString text);
private slots:
    void on_sendBtn_clicked();

//    void on_fontComboBox_currentFontChanged(const QFont &f);

//    void on_comboBox_currentIndexChanged(const QString &arg1);

//    void on_boldToolButton_clicked(bool checked);

//    void on_italicToolButton_clicked(bool checked);

//    void on_lineToolButton_clicked(bool checked);

    void on_colorToolButton_clicked(bool checked);

    void on_picToolButton_clicked(bool checked);

    void on_fileToolButton_clicked(bool checked);

    void on_fontToolButton_clicked(bool checked);

    void on_closeBtn_clicked(bool checked);

    void on_actionClose_triggered();

    void on_emotionToolButton_clicked(bool checked);

private:
    Ui::ChatWindow *ui;
    QWebEngineView      *webView;
    QString otherId;
    QString otherNick;
    QString otherRemark;

    QString webContent;
    TIMConversationHandle convHandle;

    QVector<TIMElemType> sendMsgTypeList;

    void GetConversation();

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // CHATWINDOW_H
