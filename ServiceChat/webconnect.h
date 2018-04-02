#ifndef WEBCONNECT_H
#define WEBCONNECT_H

#include <QObject>

class WebConnect : public QObject
{
    Q_OBJECT
private:
    explicit WebConnect(QObject *parent = nullptr);
public:
    static WebConnect &Instance();
signals:
    void AddContent(const QString &title, const QString &msg);
    void AddFileDesc(const QString &title, const QString &fileName, const QString &filePath, const QString &folderPath);
public slots:
    void ReceiveText(const QString &text);
    void OpenFolder(const QString &path);
    void OpenFile(const QString &path);
};

#endif // WEBCONNECT_H
