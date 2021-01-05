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
    QStringList usersToDelete = getSelectedUsers();

    for (const auto &user: usersToDelete)
    {
        DbManager::deleteUser(user);
    }

    resetForm();
}

void ManageUserView::on_PasswordResetButton_clicked()
{
    QStringList usersToResetPassword = getSelectedUsers();

    for (const auto &user: usersToResetPassword)
    {
        DbManager::changePassword(user, "");
    }

    resetForm();
}

void ManageUserView::on_UnblockButton_clicked()
{
    QStringList usersToUnblock = getSelectedUsers();

    for (const auto &user: usersToUnblock)
    {
        DbManager::unblockUser(user);
    }

    resetForm();
}

QStringList ManageUserView::getSelectedUsers()
{
    QModelIndexList list = ui->UserTableView->selectionModel()->selectedRows();

    QStringList selectedUsers;

    foreach(const QModelIndex &index, list)
    {
        selectedUsers.append(index.data(Qt::DisplayRole).toString());
    }

    qDebug() << "Manage Users: selected Users: " << selectedUsers;
    return selectedUsers;
}
