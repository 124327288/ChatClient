#include "sessionlistmodel.h"
#include <QtAlgorithms>
#include <Tim/timtool.h>
#include <algorithm>
SessionListModel::SessionListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(&TimTool::Instance(), &TimTool::NewMsg, this, &SessionListModel::AddSingleSession);
    connect(&TimTool::Instance(), &TimTool::NewConversation, this, &SessionListModel::AddSingleSession);
}

int SessionListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_sessionList.count();
}

QVariant SessionListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch (role)
    {
//    case Qt::DisplayRole:
    case Role::RoleID:
        return m_sessionList[index.row()].id;
    case Role::RoleNick:
        return m_sessionList[index.row()].nick;
    default:
        return QVariant();
    }
}

bool SessionListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool SessionListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

void SessionListModel::Sort()
{
//    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    qSort(m_sessionList.begin(), m_sessionList.end(), [](const Session &a, const Session &b){
        return a.time > b.time;
    });
//    endInsertRows();
}

void SessionListModel::UpdateData()
{
    beginInsertRows(QModelIndex(), rowCount(), TimTool::Instance().getContentMap().size());
    m_sessionList.clear();
    for(auto i = TimTool::Instance().getContentMap().begin(); i != TimTool::Instance().getContentMap().end(); ++i)
    {
        Session s;
        s.id = i.key();
        time_t t = 0;
        for(auto content : i.value())
        {
            t = std::max(static_cast<time_t>(content.time), t);
        }
        s.time = t;
        m_sessionList += s;
    }
    Sort();
    endInsertRows();
}

bool SessionListModel::IsContainInSessionList(const QString &id)
{
    for(Session s : m_sessionList)
    {
        if(s.id == id)
        {
            return true;
        }
    }
    return false;
}

void SessionListModel::AddSingleSession(const QString &id, const QString &nick, time_t time)
{
    if(IsContainInSessionList(id))
    {
        UpdateSingleSession(id, nick, time);
        return;
    }
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + 1);
    m_sessionList += {id, nick, time};
    Sort();
    endInsertRows();
}

void SessionListModel::UpdateSingleSession(const QString &id, const QString &nick, time_t time)
{
    beginResetModel();
    for(Session s : m_sessionList)
    {
        if(s.id == id)
        {
            s.nick = nick;
            s.time = time;
        }
    }
    endResetModel();
}
