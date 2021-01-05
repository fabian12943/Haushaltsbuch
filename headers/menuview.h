#ifndef MENUVIEW_H
#define MENUVIEW_H

#include <QWidget>

namespace Ui { class MenuView; }

class MenuView : public QWidget
{
    Q_OBJECT

public:
    explicit MenuView(QWidget *parent = 0);
    ~MenuView();
    void loadSpecificMenu();
    void reset();

private:
    Ui::MenuView *ui;
    void updateTable();
    QList<int> getSelectedTransactions();
    double getSaldo();

signals:
    void Logout();
    void Info();
    void Password();
    void CreateUser();
    void ManageUser();
    void Category();
    void Transaction();
    void PayOption();
    void ChangeTransaction(QStringList);

private slots:
    void on_infoButton_clicked();
    void on_passwordButton_clicked();
    void on_payoptionButton_clicked();
    void on_createUserButton_clicked();
    void on_manageUserButton_clicked();
    void on_categoryButton_clicked();
    void on_transactionButton_clicked();
    void on_logoutButton_clicked();
    void on_toDate_userDateChanged(const QDate &date);
    void on_fromDate_userDateChanged(const QDate &date);
    void on_categoryCheckBox_stateChanged(int arg1);
    void on_groupCheckBox_stateChanged(int arg1);
    void on_categoryComboBox_currentTextChanged(const QString &);
    void on_DeleteButton_clicked();
    void on_transactionsTable_doubleClicked(const QModelIndex &index);
};

#endif // MENUVIEW_H
