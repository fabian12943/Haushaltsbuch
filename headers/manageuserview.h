#ifndef MANAGEUSERVIEW_H
#define MANAGEUSERVIEW_H

#include <QWidget>

namespace Ui { class ManageUserView; }

class ManageUserView : public QWidget
{
    Q_OBJECT

public:
    explicit ManageUserView(QWidget *parent = 0);
    ~ManageUserView();
    void resetForm();

signals:
    void Back();

private slots:
    void on_BackButton_clicked();

    void on_DeleteButton_clicked();

    void on_PasswordResetButton_clicked();

    void on_UnblockButton_clicked();

private:
    Ui::ManageUserView *ui;
    QStringList getSelectedUsers();
};

#endif // MANAGEUSERVIEW_H
