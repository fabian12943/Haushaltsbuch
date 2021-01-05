#ifndef NOPASSWORDLOGINVIEW_H
#define NOPASSWORDLOGINVIEW_H

#include <QWidget>
#include "popupwidget.h"

namespace Ui { class NoPasswordLoginView; }

class NoPasswordLoginView : public QWidget
{
    Q_OBJECT

public:
    explicit NoPasswordLoginView(QWidget *parent = 0);
    ~NoPasswordLoginView();
    void loadName();

signals:
    void Login();
    void Back();

private slots:
    void on_LoginButton_clicked();
    void on_BackButton_clicked();
    void resetForm();

    void on_PasswordLine_editingFinished();

    void on_ConfirmPasswordLine_editingFinished();

    void on_ConfirmPasswordLine_returnPressed();

private:
    Ui::NoPasswordLoginView *ui;
    PopUpWidget *passwordPopUp;
};

#endif // NOPASSWORDNOPASSWORDLOGINVIEW_H
