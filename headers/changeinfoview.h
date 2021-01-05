#ifndef CHANGEINFOVIEW_H
#define CHANGEINFOVIEW_H

#include <QWidget>
#include "popupwidget.h"

namespace Ui { class ChangeInfoView; }

class ChangeInfoView : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeInfoView(QWidget *parent = 0);
    ~ChangeInfoView();
    void updateUI();

signals:
    void Back();

private slots:
    void on_BackButton_clicked();
    void on_ConfirmButton_clicked();
    void on_firstNameLine_editingFinished();
    void on_lastNameLine_editingFinished();

private:
    Ui::ChangeInfoView *ui;
    PopUpWidget *firstnamePopUp;
    PopUpWidget *lastnamePopUp;
};

#endif // CHANGEINFOVIEW_H
