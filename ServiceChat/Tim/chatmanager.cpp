#include "chatmanager.h"

ChatManager::ChatManager()
{

}

ChatManager &ChatManager::Instance()
{
    static ChatManager instance;
    return instance;
}

void ChatManager::AddToChatMap(const QString &id, const ChatContentIM &content)
{
    DEBUG_VAR(content.text);
    chatMap[id] += content;
}

QMap<QString, QVector<ChatContentIM> > &ChatManager::getChatMap()
{
    return chatMap;
}

void ChatManager::setChatMap(const QMap<QString, QVector<ChatContentIM> > &value)
{
    chatMap = value;
}
