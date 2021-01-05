#include "user.h"
#include "dbmanager.h"

User::User()
{
}

void User::setEmail(QString email)
{
    this->email = email;
}

QString User::getEmail()
{
    return this->email;
}

QString User::getFirstName()
{
    return DbManager::getFirstName(email);
}

QString User::getLastName()
{
    return DbManager::getLastName(email);
}

QString User::getBirthdate()
{
    return DbManager::getBirthdate(email);
}

bool User::isAdmin()
{
    return DbManager::isAdmin(email);
}


