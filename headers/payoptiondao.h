#ifndef PAYOPTIONDAO_H
#define PAYOPTIONDAO_H

#include <QtSql>

class PayOptionDAO
{
public:
    PayOptionDAO();
    static bool addPayoption(const QString& email, const QString& category);
    static bool removePayoption(const QString& email, const QString& category);
    static QSqlQueryModel* getPayoptionsModel(const QString &email);
    static bool payOptionExist(const QString& email, const QString& payoption);
};

#endif // PAYOPTIONDAO_H
