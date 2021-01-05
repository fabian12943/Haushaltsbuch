#include "nopasswordloginview.h"
#include "ui_nopasswordloginview.h"
#include "global.h"
#include "inputcheck.h"
#include "hashing.h"
#include "dbmanager.h"

NoPasswordLoginView::NoPasswordLoginView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::NoPasswordLoginView)
    {
        ui->setupUi(this);

        // Initialize PopUpWidget to display error message next to Inputline if necessary
        passwordPopUp = new PopUpWidget(this);
    }

NoPasswordLoginView::~NoPasswordLoginView()
{
    delete ui;
}

void NoPasswordLoginView::on_LoginButton_clicked()
{
    QString password = ui->PasswordLine->text();
    QString confirmedPassword = ui->ConfirmPasswordLine->text();

    if (password == confirmedPassword)
    {
        if (!InputCheck::isValidPassword(password))
        {
            passwordPopUp->setPopupText("Das Passwort muss mindestens 6 Zeichen lang sein.");

            const QPoint globalPos = ui->PasswordLine->mapFromGlobal(QPoint(0, 0));
            const int posX = -globalPos.x();
            const int posY = -globalPos.y();

            passwordPopUp->setGeometry(posX + ui->PasswordLine->width(),
                posY - ui->PasswordLine->height() / 2,
                passwordPopUp->width(),
                passwordPopUp->height());

            passwordPopUp->show();
        }
        else
        {
            DbManager::changePassword(g_currentUser.getEmail(), password);

            resetForm();

            emit Login();
        }
    }
    else
    {
        passwordPopUp->setPopupText("Die Passwörter stimmen nicht überein.");

        const QPoint globalPos = ui->PasswordLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        passwordPopUp->setGeometry(posX + ui->PasswordLine->width(),
            posY - ui->PasswordLine->height() / 2,
            passwordPopUp->width(),
            passwordPopUp->height());

        passwordPopUp->show();
    }
}

void NoPasswordLoginView::on_BackButton_clicked()
{
    resetForm();
    emit Back();
}

void NoPasswordLoginView::loadName()
{
    QString firstName = g_currentUser.getFirstName();
    ui->helloNameLabel->setText("Hallo " + firstName + ",");
}

void NoPasswordLoginView::resetForm()
{
    ui->PasswordLine->setText("");
    ui->ConfirmPasswordLine->setText("");
    ui->PasswordLine->setFocus();

    passwordPopUp->hide();
}

void NoPasswordLoginView::on_PasswordLine_editingFinished()
{
    passwordPopUp->hide();
}

void NoPasswordLoginView::on_ConfirmPasswordLine_editingFinished()
{
    passwordPopUp->hide();
}

void NoPasswordLoginView::on_ConfirmPasswordLine_returnPressed()
{
    on_LoginButton_clicked();
}
