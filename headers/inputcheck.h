#ifndef INPUTCHECK_H
#define INPUTCHECK_H

#include <QString>

class InputCheck
{
public:
    InputCheck();

    static bool isValidName(QString name);
    static bool isValidEmail(QString email);
    static bool isValidPassword(QString password);
    static bool isEuroCurrency(QString amount);
};

#endif // INPUTCHECK_H
