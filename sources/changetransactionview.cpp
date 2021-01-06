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

        // Initialize PopUpWidgets to display error message next to Inputline if necessary
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
    // Save userinput to variables
    QDate transactionDate = ui->transactionDate->date();
    QString type = ui->typeComboBox->currentText();
    QString amount = ui->amountLine->text().trimmed();
    QString description = ui->descriptionLine->text().trimmed();
    QString category = ui->categoryComboBox->currentText();
    QString payoption = ui->payoptionComboBox->currentText();
    QString source = ui->sourceLine->text().trimmed();

    // All userinput valid?
    bool valid = true;

    // Check if amount is valid Euro-Currency, otherwise show error message next to the input line
    unsigned long amountInCent = 0;
    if (InputCheck::isEuroCurrency(amount))
    {
        // Check number of '0's that have to be added at the end, so the amount is a valid cent value
        QString centsToAdd = "";
        if (amount.contains(","))
        {
            if (amount.contains(QRegExp(",\\d\\d"))) centsToAdd = "";
            else centsToAdd = "0";
        }
        else centsToAdd = "00";

        // Save amount in cent to prevent rounding errors
        amountInCent = (amount.remove(QRegExp("[\\D]"))).append(centsToAdd).toLong();

        // Check if amount input is 0 or too long to be stored in a long variable, otherwise show error
        // message next to the input line
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
    else // amount is not a valid Euro-Currency -> show error message next to the input line
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

    // Check if description is not empty, otherwise show error message next to the input line
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

    // Check if category is not empty, otherwise show error message next to the input line
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

    // Check if payoption is not empty, otherwise set variable to nullptr (makes sure that value
    // in the database is 'NULL' and not ''
    if (payoption.isEmpty())
    {
        payoption = nullptr;
    }

    // Check if source is not empty, otherwise set variable to nullptr (makes sure that value
    // in the database is 'NULL' and not ''
    if (source.isEmpty())
    {
        source = nullptr;
    }

    // Check if all input is valid
    if (valid)
    {
        if (!ui->payoptionComboBox->isEnabled()) payoption = nullptr;

        // Create new transaction in the database with given input
        bool success = DbManager::addTransaction(g_currentUser.getEmail(), transactionDate,
            type, amountInCent, description, category,
            payoption, source);

        // Delete old transaction if new transaction with updated values was successfully created
        if (success) DbManager::removeTransaction(transId);

        // Reset input form to default
        resetForm();

        // Go back to Menu
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
    // Fill CategoryComboBox with the categories created by the admin
    ui->categoryComboBox->setModel(DbManager::getCategoriesModel());

    // Fill PayoptionComboBox with the payoptions created the current user
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

    // Hides ErrorPopUps if any were shown
    amountPopUp->hide();
    descriptionPopUp->hide();
    categoryPopUp->hide();
    payoptionPopUp->hide();
}

// Load the values of the seleced transaction and write them in the form
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
    if (arg1 == 2) // checkBox is checked -> User wants to set a payoption
    {
        ui->payoptionComboBox->setEnabled(true);
    }
    if (arg1 == 0) // checkBox is unchecked -> User does not want to set a payoption
    {
        ui->payoptionComboBox->setEnabled(false);
    }
}
