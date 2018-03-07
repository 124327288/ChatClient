#ifndef SESSIONLISTMODEL_H
#define SESSIONLISTMODEL_H

#include <QAbstractListModel>
#include "stdafx.h"
class SessionListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SessionListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void Sort();

private:
    QList<Session> sessionList;
};

#endif // SESSIONLISTMODEL_H
