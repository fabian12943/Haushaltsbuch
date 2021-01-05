#ifndef PAYOPTIONSVIEW_H
#define PAYOPTIONSVIEW_H

#include <QWidget>
#include "popupwidget.h"

namespace Ui { class PayOptionsView; }

class PayOptionsView : public QWidget
{
    Q_OBJECT

public:
    explicit PayOptionsView(QWidget *parent = 0);
    ~PayOptionsView();
    void resetForm();

signals:
    void Back();

private:
    Ui::PayOptionsView *ui;
    PopUpWidget *payoptionPopUp;
    PopUpWidget *payoptionListPopUp;

private slots:
    void on_BackButton_clicked();
    void on_AddButton_clicked();
    void on_payoptionLine_editingFinished();
    void on_DeleteButton_clicked();
    void on_payoptionListView_clicked(const QModelIndex &);
};

#endif // PAYOPTIONSVIEW_H
