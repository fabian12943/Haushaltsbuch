#include "loginview.h"
#include "ui_loginview.h"
#include "dbmanager.h"
#include "hashing.h"
#include "global.h"
#include <iostream>

LoginView::LoginView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::LoginView)
    {
        ui->setupUi(this);

        // Initialize PopUpWidget to display error message next to inputline if necessary
        emailPopUp = new PopUpWidget(this);
    }

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_LoginButton_clicked()
{
    // Save userinput to variables
    QString email = ui->emailLine->text().toLower().trimmed();
    QString password = ui->passwordLine->text();

    // All userinput valid?
    bool valid = true;
    // User blocked?
    bool blocked = false;

    // Check if email/user exists in the database
    if (DbManager::emailExists(email))
    {
        // Check if user is blocked
        blocked = DbManager::isBlocked(email);

        if (blocked == 1) goto Blocked;

        g_currentUser.setEmail(email);

        // Check if user has a password set for his account yet
        if (DbManager::passwordSet(email))
        {
            // Get hashed password of user from database
            QString hashedDbPassword = DbManager::getPassword(email);

            // Get salt of user from database and concat with password input
            QString saltedPassword = DbManager::getSalt(email) + password;
            // Hash the combination of salt and password input
            QByteArray hashedPassword = Hashing::hash_sha256(saltedPassword);

            // Compare hashed password from database with hashed input password
            if (hashedDbPassword == hashedPassword)
            {
                qDebug() << "Login: successful login: " << email;

                // Reset the counter for recent failed logins of the user to 0
                DbManager::resetRecentFailedLoginCounter(email);

                // Open MenuView
                emit Login();
            }
            else
            {
                qDebug() << "Login: password fail: " << email;

                // Check if the user that is trying to login is not an admin
                if (!DbManager::isAdmin(email))
                {
                    // Increase the counter for recent failed logins of the user by 1
                    DbManager::increaseRecentFailedLoginCounter(email);
                }
                valid = false;
            }
        }
        else
        {
            qDebug() << "Login: no password set yet: " << email;

            // Open NoPasswordLogin, where user can set his password
            emit NoPasswordLogin();
        }
    }
    else
    {
        qDebug() << "Login: email not found in database: " << email;
        valid = false;
    }

    // Run if an error occured with the user input
    if (!valid)
    {
        qDebug() << "Login: email/password-combination not found or user blocked";
        emailPopUp->setPopupText("Ein Nutzer mit dieser E-Mail/Kennwort-Kombination\nkonnte nicht gefunden werden!");

        Blocked:
            if (blocked == true) emailPopUp->setPopupText("Der Nutzer mit dieser E-Mail ist gesperrt, da er zu viele falsche Loginversuche hatte.\n"
                "Wende dich an den Administrator, damit dieser dich wieder entsperren kann.");;

        const QPoint globalPos = ui->emailLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        emailPopUp->setGeometry(posX + ui->emailLine->width(),
            posY - ui->emailLine->height() / 2,
            emailPopUp->width(),
            emailPopUp->height());

        emailPopUp->show();
    }
    else
    {
        resetForm();
    }
}

void LoginView::on_emailLine_editingFinished()
{
    emailPopUp->hide();
}

void LoginView::on_passwordLine_editingFinished()
{
    emailPopUp->hide();
}

void LoginView::on_passwordLine_returnPressed()
{
    on_LoginButton_clicked();
}

void LoginView::resetForm()
{
    ui->emailLine->setText("");
    ui->passwordLine->setText("");
    ui->emailLine->setFocus();

    emailPopUp->hide();
}
