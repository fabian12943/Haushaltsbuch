#include "manageuserview.h"
#include "ui_manageuserview.h"
#include "dbmanager.h"

ManageUserView::ManageUserView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ManageUserView)
    {
        ui->setupUi(this);
    }

ManageUserView::~ManageUserView()
{
    delete ui;
}

void ManageUserView::resetForm()
{
    ui->UserTableView->setModel(DbManager::getUserModel());
    ui->UserTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->UserTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->UserTableView->verticalHeader()->hide();

}

void ManageUserView::on_BackButton_clicked()
{
    resetForm();
    emit Back();
}

void ManageUserView::on_DeleteButton_clicked()
{
    // Get Array of all selected users in the table
    QStringList usersToDelete = getSelectedUsers();

    // Delete all selected users
    for (const auto &user: usersToDelete)
    {
        DbManager::deleteUser(user);
    }

    // Reset all forms to default values
    resetForm();
}

void ManageUserView::on_PasswordResetButton_clicked()
{
    // Get Array of all selected users in the table
    QStringList usersToResetPassword = getSelectedUsers();

    // Reset password of all selected users
    for (const auto &user: usersToResetPassword)
    {
        DbManager::changePassword(user, "");
    }

    // Reset all forms to default values
    resetForm();
}

void ManageUserView::on_UnblockButton_clicked()
{
    // Get Array of all selected users in the table
    QStringList usersToUnblock = getSelectedUsers();

    // Unblock all selected users
    for (const auto &user: usersToUnblock)
    {
        DbManager::unblockUser(user);
    }

    // Reset all forms to default values
    resetForm();
}

QStringList ManageUserView::getSelectedUsers()
{
    // Save indexes of all selected rows
    QModelIndexList list = ui->UserTableView->selectionModel()->selectedRows();

    QStringList selectedUsers;

    // Save all emails as strings from the selected users
    foreach(const QModelIndex &index, list)
    {
        selectedUsers.append(index.data(Qt::DisplayRole).toString());
    }

    qDebug() << "Manage Users: selected Users: " << selectedUsers;
    return selectedUsers;
}

void ManageUserView::on_searchLine_textChanged(const QString &searchInput)
{
    ui->UserTableView->setModel(DbManager::getUserModel(searchInput));
}
