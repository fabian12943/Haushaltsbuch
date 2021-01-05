#include "changeinfoview.h"
#include "ui_changeinfoview.h"
#include "global.h"
#include "inputcheck.h"
#include "dbmanager.h"

ChangeInfoView::ChangeInfoView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ChangeInfoView)
    {
        ui->setupUi(this);

        // Prevents new birthdate from being in the future
        ui->birthDate->setMaximumDate(QDate::currentDate());

        // Initialize PopUpWidgets to display error message next to Inputline if necessary
        firstnamePopUp = new PopUpWidget(this);
        lastnamePopUp = new PopUpWidget(this);
    }

ChangeInfoView::~ChangeInfoView()
{
    delete ui;
}

// Fills the form with the data of the current user
void ChangeInfoView::updateUI()
{
    ui->MailLabel->setText(g_currentUser.getEmail());

    QString firstName = g_currentUser.getFirstName();
    ui->firstNameLine->setText(firstName);
    ui->firstNameLine->setPlaceholderText(firstName);

    QString lastName = g_currentUser.getLastName();
    ui->lastNameLine->setText(lastName);
    ui->lastNameLine->setPlaceholderText(lastName);

    QDate birthdate = QDate::fromString(g_currentUser.getBirthdate(), "dd.MM.yyyy");
    ui->birthDate->setDate(birthdate);

    ui->firstNameLine->setFocus();
}

void ChangeInfoView::on_BackButton_clicked()
{
    updateUI();

    // Hides error messages if any were shown
    firstnamePopUp->hide();
    lastnamePopUp->hide();

    emit Back();
}

void ChangeInfoView::on_ConfirmButton_clicked()
{
    // Save Userinput to Variables
    QString firstname = ui->firstNameLine->text().toLower().trimmed();
    firstname[0] = firstname[0].toUpper();
    QString lastname = ui->lastNameLine->text().toLower().trimmed();
    lastname[0] = lastname[0].toUpper();
    QString birthdate = ui->birthDate->text();

    // All userinputs valid?
    bool valid = true;

    // Check if firstname is valid, otherwise show error message next to input line
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

    // Check if lastname is valid, otherwise show error message next to input line
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

    // Check if all userinput is valid
    if (valid)
    {
        // Updates data of user in database
        DbManager::updateUser(g_currentUser.getEmail(), firstname, lastname, birthdate);

        // updates UI with the new user info
        updateUI();

        // hides any error messages if shown
        firstnamePopUp->hide();
        lastnamePopUp->hide();

        // goes back to Menu
        emit Back();
    }
}

void ChangeInfoView::on_firstNameLine_editingFinished()
{
    firstnamePopUp->hide();
}

void ChangeInfoView::on_lastNameLine_editingFinished()
{
    lastnamePopUp->hide();
}
