#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>

class DbManager
{
public:
    DbManager();
    ~DbManager();
    bool isOpen() const;
    bool createTables();
    void deleteAllTables();
    bool deleteTable(QString tablename);
    bool insertTestData();

    // UserDAO
    static bool addUser(const QString& email, const QString& firstname,
                    const QString& lastname, const QString& password,
                    const QString& birthdate, const int isAdmin);
    static bool updateUser(const QString& email, const QString& firstname,
                           const QString& lastname, const QString& birthdate);
    static bool deleteUser(const QString& email);
    static bool blockUser(const QString& email);
    static bool unblockUser(const QString& email);
    static bool increaseRecentFailedLoginCounter(const QString& email);
    static bool resetRecentFailedLoginCounter(const QString& email);
    static bool emailExists(const QString& email);
    static bool adminExists();
    static bool passwordSet(const QString& email);
    static QString getPassword(const QString& email);
    static QString getFirstName(const QString& email);
    static QString getLastName(const QString& email);
    static QString getBirthdate(const QString& email);
    static QString getSalt(const QString& email);
    static int getFailedLoginCounter(const QString& email);
    static bool isAdmin(const QString& email);
    static bool isBlocked(const QString& email);
    static bool changePassword(const QString& email, const QString&password);
    static QSqlQueryModel* getUserModel();

    // CategoryDAO
    static bool addCategory(const QString& email, const QString& category);
    static bool removeCategory(const QString& email, const QString& category);
    static QSqlQueryModel* getCategoriesModel();
    static bool categoryExist(const QString& email, const QString& payoption);

    // PayOptionDAO
    static bool addPayoption(const QString& email, const QString& category);
    static bool removePayoption(const QString& email, const QString& category);
    static QSqlQueryModel* getPayoptionsModel(const QString &email);
    static bool payOptionExist(const QString& email, const QString& payoption);

    // TransactionDAO
    static bool addTransaction(const QString &email, const QDate &date, const QString &type,
                               const int amountInCent, const QString &description,
                               const QString &category, const QString &payoption = nullptr,
                               const QString &source = nullptr);
    static bool removeTransaction(const int &transId);
    static QSqlQueryModel* getTransactionsModel(const QString &email, const QDate &fromDate, const QDate &toDate,
                                                const QString &category, const bool &grouped);

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
