#ifndef DATABASETOOL_H
#define DATABASETOOL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
//#include "table.h"
#include "stdafx.h"

class DatabaseTool
{
public:
    DatabaseTool();
    DatabaseTool &Instance();

    bool Bind(const QString &dbName, const char *type = "QSQLITE");
    bool IsBind();

    void ShowAllTableName();

    bool IsTableExist(const QString &tableName);
    bool IsTableExist(const char *tableName);

    int RowsNum(const char *tableName);
    int RowsNum(const QString &tableName);

    bool Update(const QString &tableName, const QVector<ParamType> &setList, const QVector<ParamType> &whereList);
    bool Delete(const QString &tableName, const QVector<ParamType> &whereList);

    template <typename T>
    int RowsNum(const T &t);

    template <typename T>
    bool IsTableExist(const T &t);

    template <typename T>
    bool CreateTable(const T &t);

    template <typename T>
    bool Select(QVector<T> *tableList, const QVector<ParamType> &whereList = QVector<ParamType>());

    template <typename T>
    bool Insert(const T &t);

    template <typename T>
    bool Update(const T &t, const QVector<ParamType> &setList, const QVector<ParamType> &whereList);

    template <typename T>
    void Delete(const T &t, const QVector<ParamType> &whereList);

private:
    QSqlDatabase db;
};

template<typename T>
bool DatabaseTool::IsTableExist(const T &t)
{
    return IsTableExist(t.metaObject()->className());
}

template<typename T>
bool DatabaseTool::CreateTable(const T &t)
{
    if(!IsBind())
        return false;
    const QMetaObject *metaObject = t.metaObject();
    int cnt = metaObject->propertyCount();
    QString s = QString("CREATE TABLE %1(").arg(metaObject->className());

    for(int i = 1; i < cnt; ++i)
    {
        QMetaProperty property = metaObject->property(i);
        QString name = property.name();
        QString tmp = i == 1 ? name : QString(", ") + name;
        switch (property.type())
        {
        case QVariant::Type::Int:
        case QVariant::Type::UInt:
        case QVariant::Type::LongLong:
        case QVariant::Type::ULongLong:
            tmp += " INTEGER ";
            break;
        case QVariant::Type::String:
            tmp += " TEXT ";
            break;
        }
        if(i == 1)
            tmp += " PRIMARY KEY ";
        s += tmp;
    }
    s += ")";
    QSqlQuery query;
    if(!query.prepare(s))
    {
        SQL_ERROR(query);
        return false;
    }
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

template<typename T>
bool DatabaseTool::Select(QVector<T> *tableList, const QVector<ParamType> &whereList)
{
    if(!IsBind())
        return false;
    QSqlQuery q;
    QString s;
    const QMetaObject *metaObject = T().metaObject();
    QString tableName = metaObject->className();
    if(whereList.empty())
    {
        s = QString("SELECT * FROM %1").arg(tableName);
    }
    else if(whereList.count() == 1)
    {
        ParamType p0 = whereList[0];
        s = QString("SELECT * FROM %1 WHERE %2 = ?").arg(tableName).arg(p0.name);
    }
    else
    {
        s = QString("SELECT * FROM %1 WHERE %2 = ? ").arg(tableName).arg(whereList[0].name);
        for(int i = 1; i < whereList.count(); ++i)
        {
            ParamType param = whereList[i];
            QString tmp = QString("And %1 = ? ").arg(param.name);
            s += tmp;
        }
    }
    if(!q.prepare(s))
    {
        SQL_ERROR(q);
        return false;
    }
    for(const ParamType &p : whereList)
    {
        q.addBindValue(p.value);
    }
    if(!q.exec())
    {
        SQL_ERROR(q);
        return false;
    }
    bool has = false;
    while(q.next())
    {
        if(!tableList)
            return true;
        has = true;
        T table;
        for(int i = 1; i < metaObject->propertyCount(); ++i)
        {
            QMetaProperty property = metaObject->property(i);
            property.write(&table, q.value(i-1));
        }
        tableList->push_back(table);
    }
    return has;
}

template<typename T>
bool DatabaseTool::Insert(const T &t)
{
    if(!IsBind())
        return false;
    const QMetaObject *metaObject = T().metaObject();
    QString tableName = metaObject->className();
    QString s = QString("INSERT INTO %1 ( ").arg(tableName);
    int cnt = metaObject->propertyCount();
    for(int i = 1; i < cnt; ++i)
    {
        QMetaProperty property = metaObject->property(i);
        QString name = property.name();
        QString tmp = i == 1 ? name : QString(" , ") + name;
        s += tmp;
    }
    s += " ) VALUES(";
    for(int i = 1; i < cnt; ++i)
    {
        QString tmp = i == 1 ? " ? " : " , ?";
        s += tmp;
    }
    s += " ) ";
    QSqlQuery query;
    if(!query.prepare(s))
    {
        SQL_ERROR(query);
        return false;
    }
    for(int i = 1; i < cnt; ++i)
    {
        QMetaProperty property = metaObject->property(i);
        query.addBindValue(property.read(&t));
    }
    if(!query.exec())
    {
        SQL_ERROR(query);
        return false;
    }
    return true;
}

template<typename T>
bool DatabaseTool::Update(const T &t, const QVector<ParamType> &setList, const QVector<ParamType> &whereList)
{
    return Update(t.metaObject()->className(), setList, whereList);
}

template<typename T>
void DatabaseTool::Delete(const T &t, const QVector<ParamType> &whereList)
{
    return Delete(t.metaObject()->className(), whereList);
}

template<typename T>
int DatabaseTool::RowsNum(const T &t)
{
    return RowsNum(t.metaObject()->className());
}

#endif // DATABASETOOL_H
