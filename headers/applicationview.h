#ifndef APPLICATIONVIEW_H
#define APPLICATIONVIEW_H

#include <QWidget>
#include "adminregistrationview.h"
#include "loginview.h"
#include "nopasswordloginview.h"
#include "menuview.h"
#include "addtransactionview.h"
#include "catview.h"
#include "changeinfoview.h"
#include "changepasswordview.h"
#include "payoptionsview.h"
#include "createuserview.h"
#include "manageuserview.h"
#include "changetransactionview.h"

namespace Ui {
class ApplicationView;
}

class ApplicationView : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationView(QWidget *parent = nullptr);
    ~ApplicationView();

private slots:
    void open_admin_registration();
    void open_login();
    void open_no_password_login();
    void open_menu();
    void open_info();
    void open_password();
    void open_payoption();
    void open_create_user();
    void open_manage_user();
    void open_category();
    void open_transaction();
    void open_change_transaction(QStringList values);

private:
    Ui::ApplicationView *ui;
    AdminRegistrationView _AdminRegistration;
    LoginView _Login;
    NoPasswordLoginView _NoPasswordLogin;
    MenuView _Menu;
    AddTransactionView _AddTransaction;
    CatView _Categories;
    ChangeInfoView _ChangeInfo;
    ChangePasswordView _ChangePassword;
    PayOptionsView _PayOptions;
    CreateUserView _CreateUser;
    ManageUserView _ManageUser;
    ChangeTransactionView _ChangeTransaction;
};

#endif // APPLICATIONVIEW_H
