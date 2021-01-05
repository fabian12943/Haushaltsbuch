#ifndef TRANSACTIONDAO_H
#define TRANSACTIONDAO_H

#include <QtSql>

class TransactionDAO
{
public:
    TransactionDAO();
    static bool addTransaction(const QString &email, const QDate &date, const QString &type,
                               const int amountInCent, const QString &description,
                               const QString &category, const QString &payoption = nullptr,
                               const QString &source = nullptr);
    static bool removeTransaction(const int &transId);
    static QSqlQueryModel* getTransactionsModel(const QString &email, const QDate &fromDate, const QDate &toDate,
                                                const QString &category, const bool &grouped);
};

#endif // TRANSACTIONDAO_H
