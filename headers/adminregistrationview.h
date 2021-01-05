#ifndef ADMINREGISTRATIONVIEW_H
#define ADMINREGISTRATIONVIEW_H

#include <QWidget>
#include "popupwidget.h"

namespace Ui { class AdminRegistrationView; }

class AdminRegistrationView : public QWidget
{
    Q_OBJECT

public:
    explicit AdminRegistrationView(QWidget *parent = 0);
    ~AdminRegistrationView();

private slots:
    void on_confirmButton_clicked();
    void on_emailLine_editingFinished();
    void on_firstNameLine_editingFinished();
    void on_lastNameLine_editingFinished();
    void on_passwordLine_editingFinished();
    void resetForm();

private:
    Ui::AdminRegistrationView *ui;
    PopUpWidget *firstnamePopUp;
    PopUpWidget *lastnamePopUp;
    PopUpWidget *emailPopUp;
    PopUpWidget *passwordPopUp;

signals:
    void AdminRegistration();
};

#endif // ADMINREGISTRATIONVIEW_H
