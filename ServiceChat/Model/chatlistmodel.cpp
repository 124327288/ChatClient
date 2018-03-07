#include "chatlistmodel.h"

#include <QTextBrowser>

ChatListModel::ChatListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    chatList << ChatContent{"id", "nick", "data", "time", "content"};
    chatList << ChatContent{"id", "nick", "data", "time", "content"};
    chatList << ChatContent{"id", "nick", "data", "time", "content"};
    chatList << ChatContent{"id", "nick", "data", "time", "content"};
    chatList << ChatContent{"id", "nick", "data", "time", "content"};
}

int ChatListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    // FIXME: Implement me!
    return chatList.size();
}

QVariant ChatListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    switch (role) {
    case ChatRoleID:
        return chatList[index.row()].title.id;
    case ChatRoleNick:
        return chatList[index.row()].title.nick;
    case ChatRoleData:
        return chatList[index.row()].title.data;
    case ChatRoleTime:
        return chatList[index.row()].title.time;
    case ChatRoleContent:
        return chatList[index.row()].text;
    default:
        break;
    }
    return QVariant();
}
