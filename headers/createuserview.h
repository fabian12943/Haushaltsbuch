#ifndef CREATEUSERVIEW_H
#define CREATEUSERVIEW_H

#include <QWidget>
#include "popupwidget.h"

namespace Ui { class CreateUserView; }

class CreateUserView : public QWidget
{
    Q_OBJECT

public:
    explicit CreateUserView(QWidget *parent = 0);
    ~CreateUserView();

signals:
    void Back();

private slots:
    void on_CreateButton_clicked();
    void on_BackButton_clicked();
    void resetForm();

    void on_firstNameLine_editingFinished();

    void on_lastNameLine_editingFinished();

    void on_emailLine_editingFinished();

private:
    Ui::CreateUserView *ui;
    PopUpWidget *firstnamePopUp;
    PopUpWidget *lastnamePopUp;
    PopUpWidget *emailPopUp;
};


#endif // CREATEUSERVIEW_H
