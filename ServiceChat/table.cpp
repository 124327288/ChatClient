#include "table.h"
QString Table::Account::getId() const
{
    return id;
}

void Table::Account::setId(const QString &value)
{
    id = value;
}

QString Table::Account::getPwd() const
{
    return pwd;
}

void Table::Account::setPwd(const QString &value)
{
    pwd = value;
}

QString Table::Sign::getId() const
{
    return id;
}

void Table::Sign::setId(const QString &value)
{
    id = value;
}

QString Table::Sign::getSig() const
{
    return sig;
}

void Table::Sign::setSig(const QString &value)
{
    sig = value;
}
