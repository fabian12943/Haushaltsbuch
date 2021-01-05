#ifndef CHANGEPASSWORDVIEW_H
#define CHANGEPASSWORDVIEW_H


#include <QWidget>
#include "popupwidget.h"

namespace Ui { class ChangePasswordView; }

class ChangePasswordView : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePasswordView(QWidget *parent = 0);
    ~ChangePasswordView();

signals:
    void Back();

private slots:
    void on_BackButton_clicked();

    void on_ConfirmButton_clicked();

    void on_CurrentPasswordLine_editingFinished();

    void on_NewPasswordLine_editingFinished();

    void on_ConfirmNewPasswordLine_editingFinished();

    void resetForm();

private:
    Ui::ChangePasswordView *ui;
    PopUpWidget *currentPasswordPopUp;
    PopUpWidget *newPasswordPopUp;
};

#endif // CHANGEPASSWORDVIEW_H
