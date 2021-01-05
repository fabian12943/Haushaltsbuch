#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include "popupwidget.h"

namespace Ui { class LoginView; }

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = 0);
    ~LoginView();

signals:
    void Login();
    void NoPasswordLogin();

private slots:
    void on_LoginButton_clicked();
    void on_emailLine_editingFinished();
    void on_passwordLine_editingFinished();
    void on_passwordLine_returnPressed();
    void resetForm();

private:
    Ui::LoginView *ui;
    PopUpWidget *emailPopUp;
};

#endif // LOGINVIEW_H
