#include "payoptiondao.h"

PayOptionDAO::PayOptionDAO(){}

bool PayOptionDAO::addPayoption(const QString &email, const QString &payoption)
{
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO payoptions VALUES"
        "(:email, :payoption)");
    queryAdd.bindValue(":email", email);
    queryAdd.bindValue(":payoption", payoption);

    if (queryAdd.exec())
    {
        qDebug() << "Database: payoption " << payoption << " for user " << email << +" added successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: payoption add failed: " << queryAdd.lastError();
        return false;
    }
}

bool PayOptionDAO::removePayoption(const QString &email, const QString &payoption)
{
    QSqlQuery checkQuery;

    checkQuery.prepare("SELECT *FROM transactions WHERE email = (:email) AND payoption = (:payoption)");
    checkQuery.bindValue(":email", email);
    checkQuery.bindValue(":payoption", payoption);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            qDebug() << "Payoption delete failed: " << payoption << " still used in transaction table";
            return false;
        }
    }

    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM payoptions WHERE email = (:email) AND payoption = (:payoption)");
    queryDelete.bindValue(":email", email);
    queryDelete.bindValue(":payoption", payoption);

    if (queryDelete.exec())
    {
        qDebug() << "Database: payoption " << payoption << " for user " << email << +" deleted successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: payoption delete failed: " << queryDelete.lastError();
        return false;
    }
}

QSqlQueryModel *PayOptionDAO::getPayoptionsModel(const QString &email)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT DISTINCT payoption FROM payoptions WHERE email = (:email)");
    query.bindValue(":email", email);
    if (query.exec())
    {
        model->setQuery(query);
        return model;
    }
    else
    {
        return nullptr;
    }
}

bool PayOptionDAO::payOptionExist(const QString &email, const QString &payoption)
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT payoption FROM payoptions WHERE email = (:email) AND payoption = (:payoption)");
    checkQuery.bindValue(":email", email);
    checkQuery.bindValue(":payoption", payoption);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }

    return exists;
}
