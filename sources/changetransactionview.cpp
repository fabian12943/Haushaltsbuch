#include "changetransactionview.h"
#include "ui_changetransactionview.h"
#include "dbmanager.h"
#include "global.h"
#include "inputcheck.h"
#include "limits.h"
#include <QRegExp>
#include <string>

ChangeTransactionView::ChangeTransactionView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ChangeTransactionView)
    {
        ui->setupUi(this);

        amountPopUp = new PopUpWidget(this);
        descriptionPopUp = new PopUpWidget(this);
        categoryPopUp = new PopUpWidget(this);
        payoptionPopUp = new PopUpWidget(this);
    }

ChangeTransactionView::~ChangeTransactionView()
{
    delete ui;
}

void ChangeTransactionView::on_BackButton_clicked()
{
    resetForm();
    emit Back();
}

void ChangeTransactionView::on_ConfirmButton_clicked()
{
    QDate transactionDate = ui->transactionDate->date();
    QString type = ui->typeComboBox->currentText();
    QString amount = ui->amountLine->text().trimmed();
    QString description = ui->descriptionLine->text().trimmed();
    QString category = ui->categoryComboBox->currentText();
    QString payoption = ui->payoptionComboBox->currentText();
    QString source = ui->sourceLine->text().trimmed();

    bool valid = true;

    unsigned long amountInCent = 0;
    if (InputCheck::isEuroCurrency(amount))
    {
        QString centsToAdd = "";

        if (amount.contains(","))
        {
            if (amount.contains(QRegExp(",\\d\\d"))) centsToAdd = "";
            else centsToAdd = "0";
        }
        else centsToAdd = "00";

        amountInCent = (amount.remove(QRegExp("[\\D]"))).append(centsToAdd).toLong();

        if (amountInCent == 0 || amountInCent > ULONG_MAX)
        {
            valid = false;

            if (amountInCent == 0) amountPopUp->setPopupText("Eingabewert muss mindestens 0,01€ betragen.");
            if (amountInCent > UINT_MAX) amountPopUp->setPopupText("Eingabewert ist zu groß.");

            const QPoint globalPos = ui->amountLine->mapFromGlobal(QPoint(0, 0));
            const int posX = -globalPos.x();
            const int posY = -globalPos.y();

            amountPopUp->setGeometry(posX + ui->amountLine->width(),
                posY - ui->amountLine->height() / 2,
                amountPopUp->width(),
                amountPopUp->height());

            amountPopUp->show();
        }
    }
    else
    {
        valid = false;

        amountPopUp->setPopupText("Zulässige Eingaben: '1,99', '2€', '€20.000,99', '0,67 €'");

        const QPoint globalPos = ui->amountLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        amountPopUp->setGeometry(posX + ui->amountLine->width(),
            posY - ui->amountLine->height() / 2,
            amountPopUp->width(),
            amountPopUp->height());

        amountPopUp->show();
    }

    if (description.isEmpty())
    {
        valid = false;

        descriptionPopUp->setPopupText("Beschreibung darf nicht leer sein.");

        const QPoint globalPos = ui->descriptionLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        descriptionPopUp->setGeometry(posX + ui->descriptionLine->width(),
            posY - ui->descriptionLine->height() / 2,
            descriptionPopUp->width(),
            descriptionPopUp->height());

        descriptionPopUp->show();
    }

    if (category.isEmpty())
    {
        valid = false;

        categoryPopUp->setPopupText("Keine Kategorien vom Admin angelegt.");

        const QPoint globalPos = ui->categoryComboBox->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        categoryPopUp->setGeometry(posX + ui->categoryComboBox->width(),
            posY - ui->categoryComboBox->height() / 2,
            categoryPopUp->width(),
            categoryPopUp->height());

        categoryPopUp->show();
    }

    if (payoption.isEmpty())
    {
        payoption = nullptr;
    }

    if (source.isEmpty())
    {
        source = nullptr;
    }

    if (valid)
    {
        if (!ui->payoptionComboBox->isEnabled()) payoption = nullptr;
        bool success = DbManager::addTransaction(g_currentUser.getEmail(), transactionDate,
            type, amountInCent, description, category,
            payoption, source);

        if (success) DbManager::removeTransaction(transId);

        resetForm();

        emit Back();
    }
}

void ChangeTransactionView::on_amountLine_editingFinished()
{
    amountPopUp->hide();
}

void ChangeTransactionView::on_descriptionLine_editingFinished()
{
    descriptionPopUp->hide();
}

void ChangeTransactionView::resetForm()
{
    ui->categoryComboBox->setModel(DbManager::getCategoriesModel());
    ui->payoptionComboBox->setModel(DbManager::getPayoptionsModel(g_currentUser.getEmail()));

    ui->transactionDate->setDate(QDate::currentDate());
    ui->transactionDate->setMaximumDate(QDate::currentDate());

    ui->transactionDate->setDate(QDate::currentDate());
    ui->typeComboBox->setCurrentIndex(0);
    ui->amountLine->setText("");
    ui->descriptionLine->setText("");
    ui->categoryComboBox->setCurrentIndex(0);
    ui->payoptionComboBox->setCurrentIndex(0);
    ui->sourceLine->setText("");

    amountPopUp->hide();
    descriptionPopUp->hide();
    categoryPopUp->hide();
    payoptionPopUp->hide();
}

void ChangeTransactionView::loadTransaction(QStringList values)
{
    transId = values[0].toInt();

    ui->transactionDate->setDate(QDate::fromString(values[1], "yyyy-MM-dd"));
    ui->typeComboBox->setCurrentText(values[2]);
    ui->categoryComboBox->setCurrentText(values[3]);
    if (values[4].isEmpty()) ui->checkBox->setChecked(false);
    else
    {
        ui->checkBox->setChecked(true);
        ui->payoptionComboBox->setCurrentText(values[4]);
    }
    ui->sourceLine->setText(values[5]);
    ui->sourceLine->setPlaceholderText(values[5]);
    ui->amountLine->setText(values[6].replace(".", ",") + " €");
    ui->amountLine->setPlaceholderText(values[6].replace(".", ",") + " €");
    ui->descriptionLine->setText(values[7]);
    ui->descriptionLine->setPlaceholderText(values[7]);

}

void ChangeTransactionView::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->payoptionComboBox->setEnabled(true);
    }
    if (arg1 == 0) ui->payoptionComboBox->setEnabled(false);
}
