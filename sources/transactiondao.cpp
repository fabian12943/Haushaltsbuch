#include "transactiondao.h"

TransactionDAO::TransactionDAO(){}

bool TransactionDAO::addTransaction(const QString &email, const QDate &date, const QString &type, const int amountInCent, const QString &description, const QString &category, const QString &payoption, const QString &source)
{
    QString strDate = date.toString("yyyy-MM-dd");
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO transactions"
        "(email, date, type, amountInCent, description,"
        "category, payoption, source) VALUES"
        "(:email, :date, :type, :amountInCent, :description,"
        ":category, :payoption, :source)");
    queryAdd.bindValue(":email", email);
    queryAdd.bindValue(":date", strDate);
    queryAdd.bindValue(":type", type);
    queryAdd.bindValue(":amountInCent", amountInCent);
    queryAdd.bindValue(":description", description);
    queryAdd.bindValue(":category", category);
    queryAdd.bindValue(":payoption", payoption);
    queryAdd.bindValue(":source", source);

    if (queryAdd.exec())
    {
        qDebug() << "Database: transaction " << description << " for user " << email << +" added successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: transaction add failed: " << queryAdd.lastError();
        return false;
    }
}

bool TransactionDAO::removeTransaction(const int &transId)
{
    QSqlQuery queryDelete;

    queryDelete.prepare("DELETE FROM transactions WHERE transId = (:transId)");
    queryDelete.bindValue(":transId", transId);

    if (queryDelete.exec())
    {
        qDebug() << "Database: transaction " << transId << " deleted successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: transaction delete failed: " << queryDelete.lastError();
        return false;
    }
}

QSqlQueryModel *TransactionDAO::getTransactionsModel(const QString &email, const QDate &fromDate, const QDate &toDate, const QString &category, const bool &grouped)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    if (!category.isEmpty() && !grouped)
    {
        query.prepare("SELECT transId AS `ID`, date AS `Datum`, type AS `Typ`,"
            "category AS `Kategorie`, payoption AS `Zahlungsart`,"
            "source AS `Quelle`, (amountInCent / 100.00) AS `Betrag`, description AS `Beschreibung`"
            "FROM transactions WHERE email = (:email) AND "
            "date >= (:fromDate) AND date <= (:toDate) AND category = (:category)"
            "ORDER BY date DESC");
        query.bindValue(":email", email);
        query.bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
        query.bindValue(":toDate", toDate.toString("yyyy-MM-dd"));
        query.bindValue(":category", category);
    }

    if (category.isEmpty() && grouped)
    {
        query.prepare("SELECT transId AS `ID`, date AS `Datum`, type AS `Typ`,"
            "category AS `Kategorie`, payoption AS `Zahlungsart`,"
            "source AS `Quelle`, (amountInCent / 100.00) AS `Betrag`, description AS `Beschreibung`"
            "FROM transactions WHERE email = (:email) AND "
            "date >= (:fromDate) AND date <= (:toDate) "
            "ORDER BY category ASC, date DESC");
        query.bindValue(":email", email);
        query.bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
        query.bindValue(":toDate", toDate.toString("yyyy-MM-dd"));
    }

    if (category.isEmpty() && !grouped)
    {
        query.prepare("SELECT transId AS `ID`, date AS `Datum`, type AS `Typ`,"
            "category AS `Kategorie`, payoption AS `Zahlungsart`,"
            "source AS `Quelle`, (amountInCent / 100.00) AS `Betrag`, description AS `Beschreibung`"
            "FROM transactions WHERE email = (:email) AND "
            "date >= (:fromDate) AND date <= (:toDate) "
            "ORDER BY date DESC");
        query.bindValue(":email", email);
        query.bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
        query.bindValue(":toDate", toDate.toString("yyyy-MM-dd"));
    }

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
