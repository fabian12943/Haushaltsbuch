#include "userdao.h"
#include "hashing.h"

UserDAO::UserDAO(){}

bool UserDAO::addUser(const QString &email, const QString &firstname, const QString &lastname, const QString &password, const QString &birthdate, const int isAdmin)
{
    quint32 saltQuint = QRandomGenerator::global()->generate();
    QString salt = QString::number(saltQuint);

    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO user(email, firstname, lastname,"
        "salt, birthdate, is_admin, is_blocked, recent_failed_logins) VALUES"
        "(:email,"
        ":firstname, :lastname, :salt,"
        ":birthdate, :isAdmin, 0, 0)");
    queryAdd.bindValue(":email", email);
    queryAdd.bindValue(":firstname", firstname);
    queryAdd.bindValue(":lastname", lastname);
    queryAdd.bindValue(":salt", salt);
    queryAdd.bindValue(":birthdate", birthdate);
    queryAdd.bindValue(":isAdmin", isAdmin);

    if (queryAdd.exec())
    {
        qDebug() << "Database: user " << email << " added successfully";
        changePassword(email, password);
        return true;
    }
    else
    {
        qDebug() << "Database: user add failed: " << queryAdd.lastError();
        return false;
    }
}

bool UserDAO::updateUser(const QString &email, const QString &firstname, const QString &lastname, const QString &birthdate)
{
    QSqlQuery changeQuery;
    changeQuery.prepare("UPDATE user SET firstname = (:firstname),"
        "lastname = (:lastname), birthdate = (:birthdate)"
        "WHERE email = (:email)");

    changeQuery.bindValue(":email", email);
    changeQuery.bindValue(":firstname", firstname);
    changeQuery.bindValue(":lastname", lastname);
    changeQuery.bindValue(":birthdate", birthdate);

    if (changeQuery.exec())
    {
        qDebug() << "Database: user " << email << " updated successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: user update failed: " << changeQuery.lastError();
        return false;
    }
}

bool UserDAO::deleteUser(const QString &email)
{
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM user WHERE email = (:email)");
    queryDelete.bindValue(":email", email);

    if (queryDelete.exec())
    {
        qDebug() << "Database: user " << email << " deleted successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: user delete failed: " << queryDelete.lastError();
        return false;
    }
}

bool UserDAO::blockUser(const QString &email)
{
    QSqlQuery changeQuery;
    changeQuery.prepare("UPDATE user SET is_blocked = (1)"
        "WHERE email = (:email)");

    changeQuery.bindValue(":email", email);

    if (changeQuery.exec())
    {
        qDebug() << "Database: user " << email << " blocked successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: user block failed: " << changeQuery.lastError();
        return false;
    }
}

bool UserDAO::unblockUser(const QString &email)
{
    QSqlQuery changeQuery;
    changeQuery.prepare("UPDATE user SET is_blocked = (0)"
        "WHERE email = (:email)");

    changeQuery.bindValue(":email", email);

    if (changeQuery.exec())
    {
        qDebug() << "Database: user " << email << " unblocked successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: user unblock failed: " << changeQuery.lastError();
        return false;
    }
}

bool UserDAO::increaseRecentFailedLoginCounter(const QString &email)
{
    QSqlQuery changeQuery;
    changeQuery.prepare("UPDATE user SET recent_failed_logins = (recent_failed_logins + 1)"
        "WHERE email = (:email)");

    changeQuery.bindValue(":email", email);

    if (changeQuery.exec())
    {
        qDebug() << "Database: user " << email << " failed_login_counter increased by 1";

        if (getFailedLoginCounter(email) == 3)
        {
            blockUser(email);
            resetRecentFailedLoginCounter(email);
        }

        return true;
    }
    else
    {
        qDebug() << "Database: increase of failed_login_counter failed: " << changeQuery.lastError();
        return false;
    }
}

bool UserDAO::resetRecentFailedLoginCounter(const QString &email)
{
    QSqlQuery changeQuery;
    changeQuery.prepare("UPDATE user SET recent_failed_logins = (0)"
        "WHERE email = (:email)");

    changeQuery.bindValue(":email", email);

    if (changeQuery.exec())
    {
        qDebug() << "Database: user " << email << " failed_login_counter reset";
        return true;
    }
    else
    {
        qDebug() << "Database: reset of failed_login_counter failed: " << changeQuery.lastError();
        return false;
    }
}

bool UserDAO::emailExists(const QString &email)
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT email FROM user WHERE email = (:email)");
    checkQuery.bindValue(":email", email);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }

    return exists;
}

bool UserDAO::adminExists()
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT email FROM user WHERE is_admin = 1");

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }

    return exists;
}

bool UserDAO::passwordSet(const QString &email)
{
    bool set = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT email FROM user WHERE email = (:email) AND password IS NOT NULL");
    checkQuery.bindValue(":email", email);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            set = true;
        }
    }

    return set;
}

QString UserDAO::getPassword(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT password FROM user WHERE email = (:email)");
    query.bindValue(":email", email);

    if (query.exec())
    {
        query.first();
        QString password = query.value(0).toString();
        return password;
    }
    else
    {
        return nullptr;
    }
}

QString UserDAO::getFirstName(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT firstname FROM user WHERE email = (:email)");
    query.bindValue(":email", email);

    if (query.exec())
    {
        query.first();
        QString firstname = query.value(0).toString();
        return firstname;
    }
    else
    {
        return nullptr;
    }
}

QString UserDAO::getLastName(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT lastname FROM user WHERE email = (:email)");
    query.bindValue(":email", email);

    if (query.exec())
    {
        query.first();
        QString lastname = query.value(0).toString();
        return lastname;
    }
    else
    {
        return nullptr;
    }
}

QString UserDAO::getBirthdate(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT birthdate FROM user WHERE email = (:email)");
    query.bindValue(":email", email);

    if (query.exec())
    {
        query.first();
        QString birthdate = query.value(0).toString();
        return birthdate;
    }
    else
    {
        return nullptr;
    }
}

QString UserDAO::getSalt(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT salt FROM user WHERE email = (:email)");
    query.bindValue(":email", email);

    if (query.exec())
    {
        query.first();
        QString salt = query.value(0).toString();
        return salt;
    }
    else
    {
        return nullptr;
    }
}

int UserDAO::getFailedLoginCounter(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT recent_failed_logins FROM user WHERE email = (:email)");
    query.bindValue(":email", email);

    if (query.exec())
    {
        query.first();
        int recentFailedLogins = query.value(0).toInt();
        return recentFailedLogins;
    }
    else
    {
        return -1;
    }
}

bool UserDAO::isAdmin(const QString &email)
{
    bool isAdmin = 0;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT email FROM user WHERE email = (:email) AND is_admin = 1");
    checkQuery.bindValue(":email", email);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            isAdmin = 1;
        }
    }

    return isAdmin;
}

bool UserDAO::isBlocked(const QString &email)
{
    bool isBlocked = 0;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT email FROM user WHERE email = (:email) AND is_blocked = 1");
    checkQuery.bindValue(":email", email);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            isBlocked = 1;
        }
    }

    return isBlocked;
}

bool UserDAO::changePassword(const QString &email, const QString &password)
{
    QByteArray hashedSaltedPassword = nullptr;

    if (!password.isEmpty())
    {
        QString saltedPassword = getSalt(email) + password;
        hashedSaltedPassword = Hashing::hash_sha256(saltedPassword);
    }

    QSqlQuery queryAdd;
    queryAdd.prepare("UPDATE user SET password = (:password) WHERE email = (:email)");

    queryAdd.bindValue(":email", email);
    queryAdd.bindValue(":password", hashedSaltedPassword);

    if (queryAdd.exec())
    {
        qDebug() << "Database: password of user " << email << " changed successfully";
        return true;
    }
    else
    {
        qDebug() << "Database: password change of user " << email << " failed: " << queryAdd.lastError();
        return false;
    }
}

QSqlQueryModel *UserDAO::getUserModel()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT email AS `E-Mail`, firstname AS `Vorname`, lastname AS `Nachname`,"
        "birthdate AS `Geburtsdatum`, is_blocked AS `gesperrt?`, password AS `Passwort?`"
        "FROM user WHERE is_admin = (0)");

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
