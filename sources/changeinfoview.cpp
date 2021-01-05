#include "changeinfoview.h"
#include "ui_changeinfoview.h"
#include "global.h"
#include "inputcheck.h"
#include "dbmanager.h"

ChangeInfoView::ChangeInfoView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeInfoView)
{
    ui->setupUi(this);

    ui->birthDate->setMaximumDate(QDate::currentDate());
    firstnamePopUp = new PopUpWidget(this);
    lastnamePopUp = new PopUpWidget(this);
}

ChangeInfoView::~ChangeInfoView()
{
    delete ui;
}

void ChangeInfoView::updateUI()
{
    ui->MailLabel->setText(g_currentUser.getEmail());

    QString firstName = g_currentUser.getFirstName();
    ui->firstNameLine->setText(firstName);
    ui->firstNameLine->setPlaceholderText(firstName);

    QString lastName = g_currentUser.getLastName();
    ui->lastNameLine->setText(lastName);
    ui->lastNameLine->setPlaceholderText(lastName);

    QDate birthdate = QDate::fromString(g_currentUser.getBirthdate(),"dd.MM.yyyy");
    ui->birthDate->setDate(birthdate);

    ui->firstNameLine->setFocus();
}

void ChangeInfoView::on_BackButton_clicked()
{
    updateUI();

    firstnamePopUp->hide();
    lastnamePopUp->hide();

    emit Back();
}

void ChangeInfoView::on_ConfirmButton_clicked()
{
    QString firstname = ui->firstNameLine->text().toLower().trimmed();
    firstname[0] = firstname[0].toUpper();
    QString lastname = ui->lastNameLine->text().toLower().trimmed();
    lastname[0] = lastname[0].toUpper();
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

    if (valid)
    {
        DbManager::updateUser(g_currentUser.getEmail(), firstname, lastname, birthdate);

        updateUI();

        firstnamePopUp->hide();
        lastnamePopUp->hide();

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
