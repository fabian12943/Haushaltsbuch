#ifndef USERDAO_H
#define USERDAO_H

#include <QtSql>

class UserDAO
{
public:
    UserDAO();
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
};

#endif // USERDAO_H
