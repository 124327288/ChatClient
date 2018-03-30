#ifndef EMOTIONDIALOG_H
#define EMOTIONDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QTableWidget>
#include <QBoxLayout>
#include "chatwindow.h"
class EmotionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EmotionDialog(QWidget *parent = nullptr);

    ChatWindow *getChatWindow() const;
    void setChatWindow(ChatWindow *value);

signals:

public slots:
    void OnCellClicked(int row, int column);
private:
    QTableWidget *tableWidget;
    QHBoxLayout *bottomLayout;
    QPushButton *closeBtn;
    QVBoxLayout *mainLayout;

    int horCount;
    int verCount;
    ChatWindow *chatWindow;

    // QWidget interface
protected:
    virtual void leaveEvent(QEvent *event) override;
};

#endif // EMOTIONDIALOG_H
