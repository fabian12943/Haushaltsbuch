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

        emailPopUp = new PopUpWidget(this);
    }

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_LoginButton_clicked()
{
    QString email = ui->emailLine->text().toLower().trimmed();
    QString password = ui->passwordLine->text();

    bool valid = true;
    bool blocked = false;

    if (DbManager::emailExists(email))
    {
        blocked = DbManager::isBlocked(email);

        if (blocked == 1) goto Blocked;

        g_currentUser.setEmail(email);

        if (DbManager::passwordSet(email))
        {
            QString hashedDbPassword = DbManager::getPassword(email);

            QString saltedPassword = DbManager::getSalt(email) + password;
            QByteArray hashedPassword = Hashing::hash_sha256(saltedPassword);

            if (hashedDbPassword == hashedPassword)
            {
                qDebug() << "Login: successful login: " << email;
                DbManager::resetRecentFailedLoginCounter(email);
                emit Login();
            }
            else
            {
                qDebug() << "Login: password fail: " << email;
                if (!DbManager::isAdmin(email))
                {
                    DbManager::increaseRecentFailedLoginCounter(email);
                }
                valid = false;
            }
        }
        else
        {
            qDebug() << "Login: no password set yet: " << email;
            emit NoPasswordLogin();
        }
    }
    else
    {
        qDebug() << "Login: email not found in database: " << email;
        valid = false;
    }

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
