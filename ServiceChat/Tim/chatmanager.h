#ifndef CHATMANAGER_H
#define CHATMANAGER_H
#include "stdafx.h"
#include <map>
class ChatManager
{
private:
    ChatManager();
public:
    static ChatManager &Instance();
    void AddToChatMap(const QString &id, const ChatContentIM &content);
    QMap<QString, QVector<ChatContentIM>> &getChatMap();
    void setChatMap(const QMap<QString, QVector<ChatContentIM>> &value);

private:
    QMap<QString, QVector<ChatContentIM>> chatMap;
};

#endif // CHATMANAGER_H
