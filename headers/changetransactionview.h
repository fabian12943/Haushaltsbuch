#ifndef CHANGETRANSACTIONVIEW_H
#define CHANGETRANSACTIONVIEW_H

#include <QWidget>
#include "popupwidget.h"

namespace Ui { class ChangeTransactionView; }

class ChangeTransactionView : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeTransactionView(QWidget *parent = 0);
    ~ChangeTransactionView();
    void resetForm();
    void loadTransaction(QStringList values);

signals:
    void Back();

private:
    Ui::ChangeTransactionView *ui;
    PopUpWidget *amountPopUp;
    PopUpWidget *descriptionPopUp;
    PopUpWidget *categoryPopUp;
    PopUpWidget *payoptionPopUp;
    int transId;

private slots:
    void on_BackButton_clicked();

    void on_ConfirmButton_clicked();

    void on_amountLine_editingFinished();

    void on_descriptionLine_editingFinished();

    void on_checkBox_stateChanged(int arg1);
};

#endif // CHANGETRANSACTIONVIEW_H
