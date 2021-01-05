#include "categorydao.h"

CategoryDAO::CategoryDAO() {}

bool CategoryDAO::addCategory(const QString &email, const QString &category)
{
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO categories VALUES"
        "(:email, :category)");
    queryAdd.bindValue(":email", email);
    queryAdd.bindValue(":category", category);

    if (queryAdd.exec())
    {
        qDebug() << "Database: category " << category << " for user " << email << +" added successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: category add failed: " << queryAdd.lastError();
        return false;
    }
}

bool CategoryDAO::removeCategory(const QString &email, const QString &category)
{
    // Check if category can be deleted safely or if it is still in use in the transaction table
    // If it is not save, method interrupts and returns false
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT *FROM transactions WHERE category = (:category)");
    checkQuery.bindValue(":category", category);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            qDebug() << "Category delete failed: " << category << " still used in transaction table";
            return false;
        }
    }

    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM categories WHERE email = (:email) AND category = (:category)");
    queryDelete.bindValue(":email", email);
    queryDelete.bindValue(":category", category);

    if (queryDelete.exec())
    {
        qDebug() << "Database: category " << category << " for user " << email << +" deleted successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: category delete failed: " << queryDelete.lastError();
        return false;
    }
}

QSqlQueryModel *CategoryDAO::getCategoriesModel()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT category FROM categories");

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

bool CategoryDAO::categoryExist(const QString &email, const QString &category)
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT category FROM categories WHERE email = (:email) AND category = (:category)");
    checkQuery.bindValue(":email", email);
    checkQuery.bindValue(":category", category);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }

    return exists;
}
