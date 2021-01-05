#include "inputcheck.h"

#include <QRegExp>

InputCheck::InputCheck()
{

}

bool InputCheck::isValidName(QString name)
{
    if (name.length() == 0) return false;

    QString strPatt = "\\b[A-Za-zäöü-]+\\b";
    QRegExp rx(strPatt);

    return rx.exactMatch(name);
}

bool InputCheck::isValidEmail(QString email)
{
    if (email.length() == 0) return false;

    QString strPatt = "\\b[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}\\b";
    QRegExp rx(strPatt);

    return rx.exactMatch(email);
}

bool InputCheck::isValidPassword(QString password)
{
    return (password.length() > 5);
}

bool InputCheck::isEuroCurrency(QString amount)
{
    if (amount.length() == 0) return false;

    QRegExp rx("^\\€?\\s*(\\d{1,3}[.](\\d{3}[.])*\\d{3}|\\d+)([,]\\d{1,2})?\\s*\\€?$");

    return rx.exactMatch(amount);
}


