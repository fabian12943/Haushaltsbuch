#include "changepasswordview.h"
#include "ui_changepasswordview.h"
#include "dbmanager.h"
#include "global.h"
#include "hashing.h"
#include "inputcheck.h"

ChangePasswordView::ChangePasswordView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ChangePasswordView)
    {
        ui->setupUi(this);

        // Initialize PopUpWidgets to display error message next to Inputline if necessary
        currentPasswordPopUp = new PopUpWidget(this);
        newPasswordPopUp = new PopUpWidget(this);
    }

ChangePasswordView::~ChangePasswordView()
{
    delete ui;
}

void ChangePasswordView::on_BackButton_clicked()
{
    resetForm();
    emit Back();
}

void ChangePasswordView::on_ConfirmButton_clicked()
{
    // Saves userinput to variables
    QString currentPassword = ui->CurrentPasswordLine->text();
    QString newPassword = ui->NewPasswordLine->text();
    QString confirmNewPassword = ui->ConfirmNewPasswordLine->text();

    // All userinput valid?
    bool valid = true;

    // Get hashed password of user from database
    QString hashedDbPassword = DbManager::getPassword(g_currentUser.getEmail());

    // Get salt of user from database and combine it with userinput of current password
    QString saltedCurrentPassword = DbManager::getSalt(g_currentUser.getEmail()) + currentPassword;

    // Hash the salted userinput of current password
    QByteArray hashedPassword = Hashing::hash_sha256(saltedCurrentPassword);

    // Check if passwords match, otherwise show error message next to userinput
    if (hashedDbPassword != hashedPassword)
    {
        valid = false;
        currentPasswordPopUp->setPopupText("Das derzeitige Passwort stimmt nicht.");

        const QPoint globalPos = ui->CurrentPasswordLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        currentPasswordPopUp->setGeometry(posX + ui->CurrentPasswordLine->width(),
            posY - ui->CurrentPasswordLine->height() / 2,
            currentPasswordPopUp->width(),
            currentPasswordPopUp->height());

        currentPasswordPopUp->show();
    }

    // Check if both userinputs of the new password match, otherwise show error message
    if (newPassword == confirmNewPassword)
    {
        // Check if new password is valid, otherwise show error message
        if (!InputCheck::isValidPassword(newPassword))
        {
            valid = false;
            newPasswordPopUp->setPopupText("Das Passwort muss mindestens\n6 Zeichen lang sein.");

            const QPoint globalPos = ui->NewPasswordLine->mapFromGlobal(QPoint(0, 0));
            const int posX = -globalPos.x();
            const int posY = -globalPos.y();

            newPasswordPopUp->setGeometry(posX + ui->NewPasswordLine->width(),
                posY - ui->NewPasswordLine->height() / 2,
                newPasswordPopUp->width(),
                newPasswordPopUp->height());

            newPasswordPopUp->show();
        }
    }
    else
    {
        valid = false;
        newPasswordPopUp->setPopupText("Die Passwörter stimmen nicht überein.");

        const QPoint globalPos = ui->NewPasswordLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        newPasswordPopUp->setGeometry(posX + ui->NewPasswordLine->width(),
            posY - ui->NewPasswordLine->height() / 2,
            newPasswordPopUp->width(),
            newPasswordPopUp->height());

        newPasswordPopUp->show();
    }

    // Check if all inputs are valid
    if (valid)
    {
        // Change password of user in database
        DbManager::changePassword(g_currentUser.getEmail(), newPassword);

        // Reset userinput form
        resetForm();

        // Go back to Menu
        emit Back();
    }
}

void ChangePasswordView::on_CurrentPasswordLine_editingFinished()
{
    currentPasswordPopUp->hide();
}

void ChangePasswordView::on_NewPasswordLine_editingFinished()
{
    newPasswordPopUp->hide();
}

void ChangePasswordView::on_ConfirmNewPasswordLine_editingFinished()
{
    newPasswordPopUp->hide();
}

void ChangePasswordView::resetForm()
{
    currentPasswordPopUp->hide();
    newPasswordPopUp->hide();

    ui->CurrentPasswordLine->setText("");
    ui->NewPasswordLine->setText("");
    ui->ConfirmNewPasswordLine->setText("");
}
