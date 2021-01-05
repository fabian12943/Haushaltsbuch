#include "payoptionsview.h"
#include "ui_payoptionsview.h"
#include "global.h"
#include "dbmanager.h"

PayOptionsView::PayOptionsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayOptionsView)
{
    ui->setupUi(this);

    payoptionPopUp = new PopUpWidget(this);
    payoptionListPopUp = new PopUpWidget(this);
}

PayOptionsView::~PayOptionsView()
{
    delete ui;
}

void PayOptionsView::resetForm()
{
    ui->payoptionLine->setText("");
    payoptionPopUp->hide();
    payoptionListPopUp->hide();
    ui->payoptionListView->setModel(DbManager::getPayoptionsModel(g_currentUser.getEmail()));
}

void PayOptionsView::on_BackButton_clicked()
{
    resetForm();
    emit Back();
}

void PayOptionsView::on_AddButton_clicked()
{
    QString payoption = ui->payoptionLine->text().trimmed();

    bool valid = true;

    if (payoption.isEmpty() || DbManager::payOptionExist(g_currentUser.getEmail(), payoption)){
        valid = false;

        if (payoption.isEmpty()) payoptionPopUp->setPopupText("Eingabe darf nicht leer sein.");
        if (DbManager::payOptionExist(g_currentUser.getEmail(), payoption)) payoptionPopUp->setPopupText("Zahlungsmittel bereits vorhanden.");

        const QPoint globalPos = ui->payoptionLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        payoptionPopUp->setGeometry(posX + ui->payoptionLine->width(),
                               posY - ui->payoptionLine->height() / 2,
                               payoptionPopUp->width(),
                               payoptionPopUp->height());

        payoptionPopUp->show();
    }

    if (valid)
    {
        DbManager::addPayoption(g_currentUser.getEmail(), payoption);
        resetForm();
    }
}

void PayOptionsView::on_payoptionLine_editingFinished()
{
    payoptionPopUp->hide();
}

void PayOptionsView::on_DeleteButton_clicked()
{
    bool valid = true;
    QModelIndexList list = ui->payoptionListView->selectionModel()->selectedIndexes();

    if (list.isEmpty())
    {
        valid = false;
        payoptionListPopUp->setPopupText("Keine Zahlungsmittel zum Löschen ausgewählt.");


    }
    else
    {
        QStringList payoptionsToDelete;
        QStringList failedDeletes;

        foreach(const QModelIndex &index, list){
           payoptionsToDelete.append(index.data(Qt::DisplayRole).toString());
        }

        for (const auto& payoption :payoptionsToDelete)
        {
            bool success = DbManager::removePayoption(g_currentUser.getEmail(), payoption);
            if (!success) failedDeletes.append(payoption);
        }

        if (!failedDeletes.isEmpty())
        {
            valid = false;
            payoptionListPopUp->setPopupText("Folgende Zahlungsmittel konnten nicht gelöscht werden,\n"
                                            "da noch Transaktion mit ihnen bestehen:\n\n"
                                            + failedDeletes.join('\n'));
        }

        resetForm();
    }

    if (!valid)
    {
        const QPoint globalPos = ui->payoptionListView->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        payoptionListPopUp->setGeometry(posX + ui->payoptionListView->width(),
                               posY - ui->payoptionListView->height() / 2 + 100,
                               payoptionListPopUp->width(),
                               payoptionListPopUp->height());

        payoptionListPopUp->show();
    }
}




void PayOptionsView::on_payoptionListView_clicked(const QModelIndex &index)
{
    payoptionListPopUp->hide();
}
