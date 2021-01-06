#include "dbmanager.h"
#include "userdao.h"
#include "categorydao.h"
#include "payoptiondao.h"
#include "transactiondao.h"
#include <QApplication>
#include <QtSql>
#include <QDebug>
#include <iostream>

DbManager::DbManager()
{
    // Connect to database and open it
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QApplication::applicationDirPath() + "//meinHaushalt_database.db";
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

// Create all needed tables in database if they do not exist already
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
    return UserDAO::addUser(email, firstname, lastname, password, birthdate, isAdmin);
}

bool DbManager::updateUser(const QString &email, const QString &firstname, const QString &lastname, const QString &birthdate)
{
    return UserDAO::updateUser(email, firstname, lastname, birthdate);
}

bool DbManager::deleteUser(const QString &email)
{
    return UserDAO::deleteUser(email);
}

bool DbManager::blockUser(const QString &email)
{
    return UserDAO::blockUser(email);
}

bool DbManager::unblockUser(const QString &email)
{
    return UserDAO::unblockUser(email);
}

bool DbManager::increaseRecentFailedLoginCounter(const QString &email)
{
    return UserDAO::increaseRecentFailedLoginCounter(email);
}

bool DbManager::resetRecentFailedLoginCounter(const QString &email)
{
    return UserDAO::resetRecentFailedLoginCounter(email);
}

bool DbManager::emailExists(const QString &email)
{
    return UserDAO::emailExists(email);
}

bool DbManager::adminExists()
{
    return UserDAO::adminExists();
}

bool DbManager::passwordSet(const QString &email)
{
    return UserDAO::passwordSet(email);
}

QString DbManager::getPassword(const QString &email)
{
    return UserDAO::getPassword(email);
}

QString DbManager::getFirstName(const QString &email)
{
    return UserDAO::getFirstName(email);
}

QString DbManager::getLastName(const QString &email)
{
    return UserDAO::getLastName(email);
}

QString DbManager::getBirthdate(const QString &email)
{
    return UserDAO::getBirthdate(email);
}

QString DbManager::getSalt(const QString &email)
{
    return UserDAO::getSalt(email);
}

int DbManager::getFailedLoginCounter(const QString &email)
{
    return UserDAO::getFailedLoginCounter(email);
}

bool DbManager::isAdmin(const QString &email)
{
    return UserDAO::isAdmin(email);
}

bool DbManager::isBlocked(const QString &email)
{
    return UserDAO::isBlocked(email);
}

bool DbManager::changePassword(const QString &email, const QString &password)
{
    return UserDAO::changePassword(email, password);
}

QSqlQueryModel *DbManager::getUserModel()
{
    return UserDAO::getUserModel();
}

bool DbManager::addCategory(const QString &email, const QString &category)
{
    return CategoryDAO::addCategory(email, category);
}

bool DbManager::removeCategory(const QString &email, const QString &category)
{
    return CategoryDAO::removeCategory(email, category);
}

QSqlQueryModel *DbManager::getCategoriesModel()
{
    return CategoryDAO::getCategoriesModel();
}

bool DbManager::categoryExist(const QString &email, const QString &category)
{
    return CategoryDAO::categoryExist(email, category);
}

bool DbManager::addPayoption(const QString &email, const QString &payoption)
{
    return PayOptionDAO::addPayoption(email, payoption);
}

bool DbManager::removePayoption(const QString &email, const QString &payoption)
{
    return PayOptionDAO::removePayoption(email, payoption);
}

QSqlQueryModel *DbManager::getPayoptionsModel(const QString &email)
{
    return PayOptionDAO::getPayoptionsModel(email);
}

bool DbManager::payOptionExist(const QString &email, const QString &payoption)
{
    return PayOptionDAO::payOptionExist(email, payoption);
}

bool DbManager::addTransaction(const QString &email, const QDate &date, const QString &type, const int amountInCent, const QString &description, const QString &category, const QString &payoption, const QString &source)
{
    return TransactionDAO::addTransaction(email, date, type, amountInCent, description, category, payoption, source);
}

bool DbManager::removeTransaction(const int &transId)
{
    return TransactionDAO::removeTransaction(transId);
}

QSqlQueryModel *DbManager::getTransactionsModel(const QString &email, const QDate &fromDate, const QDate &toDate, const QString &category, const bool &grouped)
{
    return TransactionDAO::getTransactionsModel(email, fromDate, toDate, category, grouped);
}
