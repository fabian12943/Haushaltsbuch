#include "menuview.h"
#include "ui_menuview.h"
#include "global.h"
#include "dbmanager.h"
#include "numberformatdelegate.h"
#include <QDebug>

MenuView::MenuView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::MenuView)
    {
        ui->setupUi(this);

        ui->toDate->setDate(QDate::currentDate());
        ui->toDate->setMaximumDate(QDate::currentDate());
        ui->fromDate->setDate(QDate::currentDate());
        ui->fromDate->setDate(ui->fromDate->date().addYears(-1));
        ui->fromDate->setMaximumDate(QDate::currentDate());

        // Make table columns resizable for the user
        ui->transactionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        // Stretch last column to fit the entire table
        ui->transactionsTable->horizontalHeader()->resizeSection(7, QHeaderView::Stretch);
        // Allow user to select several rows at once
        ui->transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        // Hide the vertical header
        ui->transactionsTable->verticalHeader()->hide();
        // Format currency
        ui->transactionsTable->setItemDelegateForColumn(6, new NumberFormatDelegate(this));
        // Hide first column containing email of user
        ui->transactionsTable->setColumnHidden(0, true);
    }

MenuView::~MenuView()
{
    delete ui;
}

void MenuView::loadSpecificMenu()
{
    // Alter Menu to only show admin tools if admin is logged in
    if (!g_currentUser.isAdmin())
    {
        ui->adminToolsLabel->setVisible(false);
        ui->createUserButton->close();
        ui->manageUserButton->close();
        ui->categoryButton->close();
    }
    else
    {
        ui->adminToolsLabel->setVisible(true);
        ui->createUserButton->show();
        ui->manageUserButton->show();
        ui->categoryButton->show();
    }

    if (g_currentUser.getFirstName().length() < 11)
    {
        ui->nameLabel->setText(g_currentUser.getFirstName().toUpper());
    }
    else // Abbreviate name of user to only initials if too long
    {
        QString name = g_currentUser.getFirstName()[0].toUpper() + '.' + ' ' + g_currentUser.getLastName()[0].toUpper() + '.';
        ui->nameLabel->setText(name);
    }

    ui->categoryComboBox->setModel(DbManager::getCategoriesModel());

    updateTable();
}

void MenuView::reset()
{
    ui->toDate->setDate(QDate::currentDate());
    ui->toDate->setMaximumDate(QDate::currentDate());
    ui->fromDate->setDate(QDate::currentDate());
    ui->fromDate->setDate(ui->fromDate->date().addYears(-1));
    ui->fromDate->setMaximumDate(QDate::currentDate());

    ui->categoryCheckBox->setChecked(false);
    ui->groupCheckBox->setChecked(false);
    ui->categoryComboBox->setCurrentIndex(0);
}

void MenuView::updateTable()
{
    if (ui->categoryCheckBox->isChecked() && !ui->groupCheckBox->isChecked())
    {
        ui->transactionsTable->setModel(DbManager::getTransactionsModel(g_currentUser.getEmail(), ui->fromDate->date(),
            ui->toDate->date(), ui->categoryComboBox->currentText(),
            false));
    }
    if (!ui->categoryCheckBox->isChecked() && ui->groupCheckBox->isChecked())
    {
        ui->transactionsTable->setModel(DbManager::getTransactionsModel(g_currentUser.getEmail(), ui->fromDate->date(),
            ui->toDate->date(), "", true));
    }
    if (!ui->categoryCheckBox->isChecked() && !ui->groupCheckBox->isChecked())
    {
        ui->transactionsTable->setModel(DbManager::getTransactionsModel(g_currentUser.getEmail(), ui->fromDate->date(),
            ui->toDate->date(), "", false));
    }

    QLocale locale;
    ui->saldoAmountLabel->setText((locale.toString(getSaldo(), 'f', 2)) + " €");
}

void MenuView::on_infoButton_clicked()
{
    emit Info();
}

void MenuView::on_passwordButton_clicked()
{
    emit Password();
}

void MenuView::on_payoptionButton_clicked()
{
    emit PayOption();
}

void MenuView::on_createUserButton_clicked()
{
    emit CreateUser();
}

void MenuView::on_manageUserButton_clicked()
{
    emit ManageUser();
}

void MenuView::on_categoryButton_clicked()
{
    emit Category();
}

void MenuView::on_transactionButton_clicked()
{
    emit Transaction();
}

void MenuView::on_logoutButton_clicked()
{
    qDebug() << "Logout: successful logout:" << g_currentUser.getEmail();
    reset();
    emit Logout();
}

void MenuView::on_toDate_userDateChanged(const QDate &date)
{
    QDate fromDate = ui->fromDate->date();
    QDate toDate = date;

    if (toDate < fromDate)
    {
        ui->fromDate->setDate(date.addMonths(-1));
    }

    updateTable();
}

void MenuView::on_fromDate_userDateChanged(const QDate &date)
{
    QDate fromDate = date;
    QDate toDate = ui->toDate->date();

    if (fromDate > toDate)
    {
        ui->toDate->setDate(date.addMonths(1));
    }

    updateTable();
}

void MenuView::on_categoryCheckBox_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->groupCheckBox->setChecked(false);
        ui->categoryComboBox->setEnabled(true);
    }
    if (arg1 == 0)
    {
        ui->categoryComboBox->setEnabled(false);
    }

    updateTable();
}

void MenuView::on_groupCheckBox_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        ui->categoryCheckBox->setChecked(false);
    }

    updateTable();
}

void MenuView::on_categoryComboBox_currentTextChanged(const QString &)
{
    updateTable();
}

void MenuView::on_DeleteButton_clicked()
{
    QList<int> transactionsToDelete = getSelectedTransactions();

    for (const auto &transaction: transactionsToDelete)
    {
        DbManager::removeTransaction(transaction);
    }

    updateTable();
}

QList<int> MenuView::getSelectedTransactions()
{
    QModelIndexList list = ui->transactionsTable->selectionModel()->selectedRows();

    QList<int> selectedTransactions;

    foreach(const QModelIndex &index, list)
    {
        selectedTransactions.append(index.data(Qt::DisplayRole).toInt());
    }

    return selectedTransactions;
}

double MenuView::getSaldo()
{
    QAbstractItemModel *model = ui->transactionsTable->model();

    QStringList type;
    QList<double> amount;

    double saldo = 0.00;

    for (int col = 0; col < model->columnCount(); ++col)
    {
        for (int row = 0; row < model->rowCount(); ++row)
        {
            QModelIndex index = model->index(row, col);
            if (col == 2)
            {
                type.append(index.data().toString());
            }
            if (col == 6)
            {
                amount.append(index.data().toDouble());
            }
        }
    }

    for (int i = 0; i < type.length(); ++i)
    {
        if (type[i] == "Ausgabe") saldo -= amount[i];
        if (type[i] == "Einnahme") saldo += amount[i];
    }

    return saldo;
}

void MenuView::on_transactionsTable_doubleClicked(const QModelIndex &index)
{
    int row = index.row();

    QAbstractItemModel *model = ui->transactionsTable->model();

    int columnAmount = model->columnCount();

    QStringList values;

    for (int i = 0; i < columnAmount; ++i)
    {
        values.append(index.sibling(row, i).data().toString());
    }

    emit ChangeTransaction(values);
}
