#include "applicationview.h"
#include "ui_applicationview.h"
#include "dbmanager.h"

ApplicationView::ApplicationView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ApplicationView)
    {
        ui->setupUi(this);

        ui->stackedWidget->addWidget(&_AdminRegistration);
        ui->stackedWidget->addWidget(&_Login);
        ui->stackedWidget->addWidget(&_NoPasswordLogin);
        ui->stackedWidget->addWidget(&_Menu);
        ui->stackedWidget->addWidget(&_AddTransaction);
        ui->stackedWidget->addWidget(&_Categories);
        ui->stackedWidget->addWidget(&_ChangeInfo);
        ui->stackedWidget->addWidget(&_ChangePassword);
        ui->stackedWidget->addWidget(&_PayOptions);
        ui->stackedWidget->addWidget(&_CreateUser);
        ui->stackedWidget->addWidget(&_ManageUser);
        ui->stackedWidget->addWidget(&_ChangeTransaction);

        connect(&_AdminRegistration, SIGNAL(AdminRegistration()), this, SLOT(open_login()));

        connect(&_Login, SIGNAL(Login()), this, SLOT(open_menu()));
        connect(&_Login, SIGNAL(NoPasswordLogin()), this, SLOT(open_no_password_login()));

        connect(&_NoPasswordLogin, SIGNAL(Login()), this, SLOT(open_menu()));
        connect(&_NoPasswordLogin, SIGNAL(Back()), this, SLOT(open_login()));

        connect(&_Menu, SIGNAL(Transaction()), this, SLOT(open_transaction()));
        connect(&_Menu, SIGNAL(Category()), this, SLOT(open_category()));
        connect(&_Menu, SIGNAL(Info()), this, SLOT(open_info()));
        connect(&_Menu, SIGNAL(Password()), this, SLOT(open_password()));
        connect(&_Menu, SIGNAL(PayOption()), this, SLOT(open_payoption()));
        connect(&_Menu, SIGNAL(CreateUser()), this, SLOT(open_create_user()));
        connect(&_Menu, SIGNAL(ManageUser()), this, SLOT(open_manage_user()));
        connect(&_Menu, SIGNAL(ChangeTransaction(QStringList)), this, SLOT(open_change_transaction(QStringList)));

        connect(&_Menu, SIGNAL(Logout()), this, SLOT(open_login()));

        connect(&_AddTransaction, SIGNAL(Back()), this, SLOT(open_menu()));

        connect(&_Categories, SIGNAL(Back()), this, SLOT(open_menu()));

        connect(&_ChangeInfo, SIGNAL(Back()), this, SLOT(open_menu()));

        connect(&_ChangePassword, SIGNAL(Back()), this, SLOT(open_menu()));

        connect(&_PayOptions, SIGNAL(Back()), this, SLOT(open_menu()));

        connect(&_CreateUser, SIGNAL(Back()), this, SLOT(open_menu()));

        connect(&_ManageUser, SIGNAL(Back()), this, SLOT(open_menu()));

        connect(&_ChangeTransaction, SIGNAL(Back()), this, SLOT(open_menu()));

        if (DbManager::adminExists() == false)
        {
            open_admin_registration();
        }
        else
        {
            open_login();
        }
    }

ApplicationView::~ApplicationView()
{
    delete ui;
}

void ApplicationView::open_admin_registration()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void ApplicationView::open_login()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void ApplicationView::open_no_password_login()
{
    _NoPasswordLogin.loadName();
    ui->stackedWidget->setCurrentIndex(2);
}

void ApplicationView::open_menu()
{
    _Menu.loadSpecificMenu();
    ui->stackedWidget->setCurrentIndex(3);
}

void ApplicationView::open_info()
{
    _ChangeInfo.updateUI();
    ui->stackedWidget->setCurrentIndex(6);
}

void ApplicationView::open_password()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void ApplicationView::open_payoption()
{
    _PayOptions.resetForm();
    ui->stackedWidget->setCurrentIndex(8);
}

void ApplicationView::open_create_user()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void ApplicationView::open_manage_user()
{
    _ManageUser.resetForm();
    ui->stackedWidget->setCurrentIndex(10);
}

void ApplicationView::open_category()
{
    _Categories.resetForm();
    ui->stackedWidget->setCurrentIndex(5);
}

void ApplicationView::open_transaction()
{
    _AddTransaction.resetForm();
    ui->stackedWidget->setCurrentIndex(4);
}

void ApplicationView::open_change_transaction(QStringList values)
{
    _ChangeTransaction.resetForm();
    _ChangeTransaction.loadTransaction(values);
    ui->stackedWidget->setCurrentIndex(11);
}
