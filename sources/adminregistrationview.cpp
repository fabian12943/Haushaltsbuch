#include "adminregistrationview.h"
#include "ui_adminregistrationview.h"
#include "dbmanager.h"
#include "inputcheck.h"
#include "loginview.h"
#include <QtSql>
#include <QToolTip>
#include <iostream>

AdminRegistrationView::AdminRegistrationView(QWidget *parent): QWidget(parent), ui(new Ui::AdminRegistrationView)
{
    ui->setupUi(this);

    ui->birthDate->setDate(QDate::currentDate());
    ui->birthDate->setMaximumDate(QDate::currentDate());

    // Initialize PopUpWidgets to display error message next to Inputline if necessary
    firstnamePopUp = new PopUpWidget(this);
    lastnamePopUp = new PopUpWidget(this);
    emailPopUp = new PopUpWidget(this);
    passwordPopUp = new PopUpWidget(this);

}

AdminRegistrationView::~AdminRegistrationView()
{
    delete ui;
}

void AdminRegistrationView::on_confirmButton_clicked()
{
    // Save Userinput to Variables
    QString firstname = ui->firstNameLine->text().toLower().trimmed();
    firstname[0] = firstname[0].toUpper();
    QString lastname = ui->lastNameLine->text().toLower().trimmed();
    lastname[0] = lastname[0].toUpper();
    QString email = ui->emailLine->text().toLower().trimmed();
    QString password = ui->passwordLine->text();
    QString birthdate = ui->birthDate->text();

    // all userinput valid?
    bool valid = true;

    // Check if firstname is valid otherwise show error message next to the input line
    if (!InputCheck::isValidName(firstname))
    {
        valid = false;

        firstnamePopUp->setPopupText("Der eingegebene Vorname ist nicht zulässig.");

        const QPoint globalPos = ui->firstNameLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        firstnamePopUp->setGeometry(posX + ui->firstNameLine->width(),
            posY - ui->firstNameLine->height() / 2,
            firstnamePopUp->width(),
            firstnamePopUp->height());

        firstnamePopUp->show();
    }

    // Check if lastname is valid otherwise show error message next to the input line
    if (!InputCheck::isValidName(lastname))
    {
        valid = false;

        lastnamePopUp->setPopupText("Der eingegebene Nachname ist nicht zulässig.");

        const QPoint globalPos = ui->lastNameLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        lastnamePopUp->setGeometry(posX + ui->lastNameLine->width(),
            posY - ui->lastNameLine->height() / 2,
            lastnamePopUp->width(),
            lastnamePopUp->height());

        lastnamePopUp->show();
    }

    // Check if email is valid and not already in use, otherwise show error message next to the input line
    if (!InputCheck::isValidEmail(email) || DbManager::emailExists(email))
    {
        valid = false;

        if (!InputCheck::isValidEmail(email))
        {
            emailPopUp->setPopupText("Die eingegebene E-Mail ist ungültig.");
        }

        if (DbManager::emailExists(email))
        {
            emailPopUp->setPopupText("Die eingegebene E-Mail ist bereits in Verwendung.");
        }

        const QPoint globalPos = ui->emailLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        emailPopUp->setGeometry(posX + ui->emailLine->width(),
            posY - ui->emailLine->height() / 2,
            emailPopUp->width(),
            emailPopUp->height());

        emailPopUp->show();
    }

    // Check if password is valid otherwise show error message next to the input line
    if (!InputCheck::isValidPassword(password))
    {
        valid = false;

        passwordPopUp->setPopupText("Das Passwort muss mindestens 6 Zeichen lang sein.");

        const QPoint globalPos = ui->passwordLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        passwordPopUp->setGeometry(posX + ui->passwordLine->width(),
            posY - ui->passwordLine->height() / 2,
            passwordPopUp->width(),
            passwordPopUp->height());

        passwordPopUp->show();
    }

    // Check if all input is valid
    if (valid)
    {
        qDebug() << password;
        // Create new user in the database with given input
        DbManager::addUser(email, firstname, lastname, password, birthdate, 1);

        // Reset input form to default
        resetForm();

        // Send signal to ApplicationView to open LoginView
        emit AdminRegistration();
    }
}

void AdminRegistrationView::on_emailLine_editingFinished()
{
    emailPopUp->hide();
}

void AdminRegistrationView::on_firstNameLine_editingFinished()
{
    firstnamePopUp->hide();
}

void AdminRegistrationView::on_lastNameLine_editingFinished()
{
    lastnamePopUp->hide();
}

void AdminRegistrationView::on_passwordLine_editingFinished()
{
    passwordPopUp->hide();
}

void AdminRegistrationView::resetForm()
{
    ui->firstNameLine->setText("");
    ui->lastNameLine->setText("");
    ui->emailLine->setText("");
    ui->passwordLine->setText("");
    ui->birthDate->setDate(QDate::currentDate());

    ui->firstNameLine->setFocus();
}
