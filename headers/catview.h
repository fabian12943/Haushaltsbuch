#ifndef CATVIEW_H
#define CATVIEW_H

#include <QWidget>
#include "popupwidget.h"

namespace Ui { class CatView; }

class CatView : public QWidget
{
    Q_OBJECT

public:
    explicit CatView(QWidget *parent = 0);
    ~CatView();
    void resetForm();

signals:
    void Back();

private:
    Ui::CatView *ui;
    PopUpWidget *categoryPopUp;
    PopUpWidget *categoryListPopUp;

private slots:
    void on_BackButton_clicked();

    void on_AddButton_clicked();
    void on_categoryLine_editingFinished();
    void on_DeleteButton_clicked();
    void on_categoryListView_clicked(const QModelIndex &index);
};

#endif // CATVIEW_H

