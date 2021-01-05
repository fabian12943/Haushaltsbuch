#include "dbmanager.h"
#include "hashing.h"
#include <QApplication>
#include <QtSql>
#include <QDebug>
#include <iostream>

DbManager::DbManager()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QApplication::applicationDirPath() + "haushaltsbuch.db";
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
        QSqlQuery query;
        query.exec("PRAGMA foreign_keys = ON;");
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::createTables()
{
    QSqlQuery query;

    if (!(m_db.tables().contains(QLatin1String("user"))))
    {
        query.prepare("CREATE TABLE user"
            "(email TEXT PRIMARY KEY,"
            "firstname TEXT NOT NULL,"
            "lastname TEXT NOT NULL,"
            "password TEXT,"
            "salt TEXT NOT NULL,"
            "birthdate TEXT NOT NULL,"
            "is_admin INTEGER NOT NULL,"
            "is_blocked INTEGER NOT NULL DEFAULT 0,"
            "recent_failed_logins INTEGER NOT NULL DEFAULT 0)");

        if (!query.exec())
        {
            qDebug() << "Couldn't create the table 'user'";
            return false;
        }
    }

    if (!(m_db.tables().contains(QLatin1String("payoptions"))))
    {
        query.prepare("CREATE TABLE payoptions"
            "(email TEXT NOT NULL,"
            "payoption TEXT NOT NULL,"
            "PRIMARY KEY (email, payoption),"
            "FOREIGN KEY (email) REFERENCES user(email) ON DELETE CASCADE)");

        if (!query.exec())
        {
            qDebug() << "Couldn't create the table 'payoptions'";
            return false;
        }
    }

    if (!(m_db.tables().contains(QLatin1String("categories"))))
    {
        query.prepare("CREATE TABLE categories"
            "(email TEXT NOT NULL,"
            "category TEXT NOT NULL,"
            "PRIMARY KEY (email, category),"
            "FOREIGN KEY (email) REFERENCES user(email) ON DELETE CASCADE)");

        if (!query.exec())
        {
            qDebug() << "Couldn't create the table 'categories'";
            return false;
        }
    }

    if (!(m_db.tables().contains(QLatin1String("transactions"))))
    {
        query.prepare("CREATE TABLE transactions"
            "(transId INTEGER PRIMARY KEY AUTOINCREMENT,"
            "email TEXT NOT NULL,"
            "date TEXT NOT NULL,"
            "type TEXT NOT NULL CHECK (type IN ('Einnahme', 'Ausgabe')),"
            "amountInCent L NOT NULL,"
            "description TEXT NOT NULL,"
            "category TEXT NOT NULL,"
            "payoption TEXT,"
            "source TEXT,"
            "FOREIGN KEY (email) REFERENCES user(email) ON DELETE CASCADE)");

        if (!query.exec())
        {
            qDebug() << "Couldn't create the table 'transactions'";
            return false;
        }
    }

    return true;
}

void DbManager::deleteAllTables()
{
    QStringList tables = { "user", "payoptions", "categories", "transactions", "sqlite_sequence" };

    for (int i = 0; i < tables.size(); ++i)
    {
        deleteTable(tables[i]);
    }

    qDebug() << "Database: all tables cleaned";
}

bool DbManager::deleteTable(QString tablename)
{
    QSqlQuery deleteQuery;

    if ((m_db.tables().contains(tablename)))
    {
        deleteQuery.prepare("DELETE FROM " + tablename);

        if (!deleteQuery.exec())
        {
            qDebug() << "Database: could not delete the table " << tablename << ". Maybe the table hasn't been created yet.";
            return false;
        }
    }

    return true;
}

bool DbManager::insertTestData()
{
    addUser("rd@hs.aa", "Roland", "Dietrich", "RD", "12.05.1968", 1);
    addUser("km@hs.aa", "Klara", "Musterfrau", "KM", "23.12.1992", 0);
    addUser("mm@hs.aa", "Max", "Mustersohn", "MM", "02.01.2003", 0);

    addCategory("rd@hs.aa", "Gehaltseingang");
    addCategory("rd@hs.aa", "Lebensmittel");
    addCategory("rd@hs.aa", "Freizeit");

    addPayoption("rd@hs.aa", "Bar");
    addPayoption("rd@hs.aa", "Überweisung");
    addPayoption("rd@hs.aa", "Kreditkarte");
    addPayoption("rd@hs.aa", "Bitcoin");

    addPayoption("km@hs.aa", "Bar");
    addPayoption("km@hs.aa", "Überweisung");
    addPayoption("km@hs.aa", "Master-Card");
    addPayoption("km@hs.aa", "VISA");

    addPayoption("mm@hs.aa", "Bar");
    addPayoption("mm@hs.aa", "Überweisung");
    addPayoption("mm@hs.aa", "American Express");
    addPayoption("mm@hs.aa", "Diners Club");

    addTransaction("rd@hs.aa", QDate::fromString("20-11-2020", "d-M-yyyy"), "Ausgabe", 749, "Schwimmbadbesuch", "Freizeit", "Bar");
    addTransaction("rd@hs.aa", QDate::fromString("28-11-2020", "d-M-yyyy"), "Ausgabe", 13142, "Wocheneinkauf", "Lebensmittel", "Kreditkarte", "REWE");
    addTransaction("rd@hs.aa", QDate::fromString("12-12-2020", "d-M-yyyy"), "Ausgabe", 299, "Gebrannte Mandeln", "Lebensmittel", "Bitcoin");
    addTransaction("rd@hs.aa", QDate::fromString("01-12-2020", "d-M-yyyy"), "Einnahme", 350000, "Gehalt", "Gehaltseingang", "Überweisung");
    addTransaction("rd@hs.aa", QDate::fromString("20-12-2020", "d-M-yyyy"), "Ausgabe", 76256, "iPad", "Freizeit");
    addTransaction("rd@hs.aa", QDate::fromString("01-01-2021", "d-M-yyyy"), "Einnahme", 350000, "Gehalt", "Gehaltseingang", "Überweisung");
    addTransaction("rd@hs.aa", QDate::fromString("02-01-2021", "d-M-yyyy"), "Ausgabe", 1599, "Netflix Monatsbeitrag", "Freizeit", nullptr, "Netflix");

    addTransaction("km@hs.aa", QDate::fromString("12-10-2020", "d-M-yyyy"), "Einnahme", 230000, "Gehalt", "Gehaltseingang", "Überweisung", "KUKA");
    addTransaction("km@hs.aa", QDate::fromString("24-11-2020", "d-M-yyyy"), "Ausgabe", 39900, "Playstation 4", "Freizeit", "Bar", "Media Markt");
    addTransaction("km@hs.aa", QDate::fromString("30-11-2020", "d-M-yyyy"), "Ausgabe", 179, "Bananen", "Lebensmittel");
    addTransaction("km@hs.aa", QDate::fromString("23-12-2020", "d-M-yyyy"), "Ausgabe", 23459, "Last Minute Weihnachtsgeschenk", "Freizeit", "VISA");
    addTransaction("km@hs.aa", QDate::fromString("01-01-2021", "d-M-yyyy"), "Ausgabe", 18900, "Computer Bildschirm", "Freizeit", "Master-Card", "Conrad");
    addTransaction("km@hs.aa", QDate::fromString("04-01-2021", "d-M-yyyy"), "Ausgabe", 2346, "Markteinkauf", "Lebensmittel", "Bar");

    addTransaction("mm@hs.aa", QDate::fromString("12-09-2020", "d-M-yyyy"), "Einnahme", 3000, "Taschengeld", "Gehaltseingang", "Bar", "Eltern");
    addTransaction("mm@hs.aa", QDate::fromString("28-11-2020", "d-M-yyyy"), "Ausgabe", 6999, "FIFA 20", "Freizeit", "American Express");
    addTransaction("mm@hs.aa", QDate::fromString("12-12-2020", "d-M-yyyy"), "Ausgabe", 1499, "Jack Daniels", "Lebensmittel", "Diners Club");

    qDebug() << "Database: test data inserted successfully";
    return true;
}

bool DbManager::addUser(const QString &email, const QString &firstname, const QString &lastname, const QString &password, const QString &birthdate, const int isAdmin)
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

bool DbManager::updateUser(const QString &email, const QString &firstname, const QString &lastname, const QString &birthdate)
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

bool DbManager::deleteUser(const QString &email)
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

bool DbManager::blockUser(const QString &email)
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

bool DbManager::unblockUser(const QString &email)
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

bool DbManager::increaseRecentFailedLoginCounter(const QString &email)
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

bool DbManager::resetRecentFailedLoginCounter(const QString &email)
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

bool DbManager::emailExists(const QString &email)
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

bool DbManager::adminExists()
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

bool DbManager::passwordSet(const QString &email)
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

QString DbManager::getPassword(const QString &email)
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

QString DbManager::getFirstName(const QString &email)
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

QString DbManager::getLastName(const QString &email)
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

QString DbManager::getBirthdate(const QString &email)
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

QString DbManager::getSalt(const QString &email)
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

int DbManager::getFailedLoginCounter(const QString &email)
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

bool DbManager::isAdmin(const QString &email)
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

bool DbManager::isBlocked(const QString &email)
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

bool DbManager::changePassword(const QString &email, const QString &password)
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

QSqlQueryModel *DbManager::getUserModel()
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

bool DbManager::addCategory(const QString &email, const QString &category)
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

bool DbManager::removeCategory(const QString &email, const QString &category)
{
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

QSqlQueryModel *DbManager::getCategoriesModel()
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

bool DbManager::categoryExist(const QString &email, const QString &category)
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

bool DbManager::addPayoption(const QString &email, const QString &payoption)
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

bool DbManager::removePayoption(const QString &email, const QString &payoption)
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

QSqlQueryModel *DbManager::getPayoptionsModel(const QString &email)
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

bool DbManager::payOptionExist(const QString &email, const QString &payoption)
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

bool DbManager::addTransaction(const QString &email, const QDate &date, const QString &type, const int amountInCent, const QString &description, const QString &category, const QString &payoption, const QString &source)
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

bool DbManager::removeTransaction(const int &transId)
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

QSqlQueryModel *DbManager::getTransactionsModel(const QString &email, const QDate &fromDate, const QDate &toDate, const QString &category, const bool &grouped)
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
