#include "changepasswordview.h"
#include "ui_changepasswordview.h"
#include "dbmanager.h"
#include "global.h"
#include "hashing.h"
#include "inputcheck.h"

ChangePasswordView::ChangePasswordView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangePasswordView)
{
    ui->setupUi(this);

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
    QString currentPassword = ui->CurrentPasswordLine->text();
    QString newPassword = ui->NewPasswordLine->text();
    QString confirmNewPassword = ui->ConfirmNewPasswordLine->text();

    bool valid = true;

    QString hashedDbPassword = DbManager::getPassword(g_currentUser.getEmail());

    QString saltedCurrentPassword = DbManager::getSalt(g_currentUser.getEmail()) + currentPassword;
    QByteArray hashedPassword = Hashing::hash_sha256(saltedCurrentPassword);

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

    if (newPassword == confirmNewPassword)
    {
        if(!InputCheck::isValidPassword(newPassword))
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

    if (valid)
    {
        DbManager::changePassword(g_currentUser.getEmail(), newPassword);

        resetForm();

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
