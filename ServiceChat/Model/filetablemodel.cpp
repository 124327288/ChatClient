#include "filetablemodel.h"
#include "stdafx.h"
FileTableModel::FileTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant FileTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical || role != Qt::DisplayRole)
        return QVariant();
    switch (section)
    {
    case 0:
        return tr("File Name");
    case 1:
        return tr("Time");
    case 2:
        return tr("More");
    default:
        break;
    }
    return QVariant();
}

bool FileTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


int FileTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 2;
}

int FileTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 3;
}

QVariant FileTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (index.column())
    {
    case 0:
        return "File12321Name";
    case 1:
        return "20180330";
    case 2:
        return "none";
    }
//    switch (role)
//    {
//    case RoleFileName:
//        return "FileName";
//    case RoleTime:
//        return "20180330";
//    case RoleMore:
//        return "none";
//    default:
//        break;
//    }
    return QVariant();
}

bool FileTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags FileTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool FileTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool FileTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool FileTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool FileTableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}
