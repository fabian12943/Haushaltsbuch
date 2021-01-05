#ifndef ADDTRANSACTIONVIEW_H
#define ADDTRANSACTIONVIEW_H

#include <QWidget>
#include "popupwidget.h"

namespace Ui { class AddTransactionView; }

class AddTransactionView : public QWidget
{
    Q_OBJECT

public:
    explicit AddTransactionView(QWidget *parent = 0);
    ~AddTransactionView();
    void resetForm();

signals:
    void Back();

private:
    Ui::AddTransactionView *ui;
    PopUpWidget *amountPopUp;
    PopUpWidget *descriptionPopUp;
    PopUpWidget *categoryPopUp;
    PopUpWidget *payoptionPopUp;

private slots:
    void on_BackButton_clicked();

    void on_ConfirmButton_clicked();

    void on_amountLine_editingFinished();

    void on_descriptionLine_editingFinished();

    void on_checkBox_stateChanged(int arg1);
};

#endif // ADDTRANSACTIONVIEW_H
