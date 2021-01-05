#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();
    void setEmail(QString email);
    QString getEmail();
    QString getFirstName();
    QString getLastName();
    QString getBirthdate();
    bool isAdmin();

private:
    QString email;

};

#endif // USER_H
