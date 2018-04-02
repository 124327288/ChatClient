#ifndef WEBCONNECT_H
#define WEBCONNECT_H

#include <QObject>

class WebConnect : public QObject
{
    Q_OBJECT
public:
    explicit WebConnect(QObject *parent = nullptr);

signals:
    void SendText(const QString &text);
public slots:
    void ReceiveText(const QString &text);
    void OpenFolder(const QString &path);
    void OpenFile(const QString &path);
};

#endif // WEBCONNECT_H
