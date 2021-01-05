#include "createuserview.h"
#include "ui_createuserview.h"
#include "inputcheck.h"
#include "dbmanager.h"

CreateUserView::CreateUserView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateUserView)
{
    ui->setupUi(this);

    ui->birthDate->setDate(QDate::currentDate());
    ui->birthDate->setMaximumDate(QDate::currentDate());

    firstnamePopUp = new PopUpWidget(this);
    lastnamePopUp = new PopUpWidget(this);
    emailPopUp = new PopUpWidget(this);
}

CreateUserView::~CreateUserView()
{
    delete ui;
}

void CreateUserView::on_BackButton_clicked()
{
    resetForm();
    emit Back();
}

void CreateUserView::resetForm()
{
    ui->firstNameLine->setText("");
    ui->lastNameLine->setText("");
    ui->emailLine->setText("");
    ui->birthDate->setDate(QDate::currentDate());

    firstnamePopUp->hide();
    lastnamePopUp->hide();
    emailPopUp->hide();
}

void CreateUserView::on_CreateButton_clicked()
{
    QString firstname = ui->firstNameLine->text().toLower().trimmed();
    firstname[0] = firstname[0].toUpper();
    QString lastname = ui->lastNameLine->text().toLower().trimmed();
    lastname[0] = lastname[0].toUpper();
    QString email = ui->emailLine->text().toLower().trimmed();
    QString birthdate = ui->birthDate->text();

    bool valid = true;

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

    if (valid)
    {
        DbManager::addUser(email, firstname, lastname, nullptr, birthdate,0);
        resetForm();
    }
}

void CreateUserView::on_firstNameLine_editingFinished()
{
    firstnamePopUp->hide();
}

void CreateUserView::on_lastNameLine_editingFinished()
{
    lastnamePopUp->hide();
}

void CreateUserView::on_emailLine_editingFinished()
{
    emailPopUp->hide();
}
