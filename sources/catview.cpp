#include "catview.h"
#include "ui_catview.h"
#include "dbmanager.h"
#include "global.h"

CatView::CatView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CatView)
    {
        ui->setupUi(this);

        categoryPopUp = new PopUpWidget(this);
        categoryListPopUp = new PopUpWidget(this);
    }

CatView::~CatView()
{
    delete ui;
}

void CatView::resetForm()
{
    ui->categoryLine->setText("");
    categoryPopUp->hide();
    categoryListPopUp->hide();
    ui->categoryListView->setModel(DbManager::getCategoriesModel());
}

void CatView::on_BackButton_clicked()
{
    resetForm();
    emit Back();
}

void CatView::on_AddButton_clicked()
{
    QString category = ui->categoryLine->text().trimmed();

    bool valid = true;

    if (category.isEmpty() || DbManager::categoryExist(g_currentUser.getEmail(), category))
    {
        valid = false;

        if (category.isEmpty()) categoryPopUp->setPopupText("Eingabe darf nicht leer sein.");
        if (DbManager::categoryExist(g_currentUser.getEmail(), category)) categoryPopUp->setPopupText("Kategorie bereits vorhanden.");

        const QPoint globalPos = ui->categoryLine->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        categoryPopUp->setGeometry(posX + ui->categoryLine->width(),
            posY - ui->categoryLine->height() / 2,
            categoryPopUp->width(),
            categoryPopUp->height());

        categoryPopUp->show();
    }

    if (valid)
    {
        DbManager::addCategory(g_currentUser.getEmail(), category);
        resetForm();
    }
}

void CatView::on_categoryLine_editingFinished()
{
    categoryPopUp->hide();
}

void CatView::on_DeleteButton_clicked()
{
    bool valid = true;
    QModelIndexList list = ui->categoryListView->selectionModel()->selectedIndexes();

    if (list.isEmpty())
    {
        valid = false;
        categoryListPopUp->setPopupText("Keine Kategorien zum Löschen ausgewählt.");
    }
    else
    {
        QStringList categoriesToDelete;
        QStringList failedDeletes;

        foreach(const QModelIndex &index, list)
        {
            categoriesToDelete.append(index.data(Qt::DisplayRole).toString());
        }

        for (const auto &category: categoriesToDelete)
        {
            bool success = DbManager::removeCategory(g_currentUser.getEmail(), category);
            if (!success) failedDeletes.append(category);
        }

        if (!failedDeletes.isEmpty())
        {
            valid = false;
            categoryListPopUp->setPopupText("Folgende Kategorien konnten nicht gelöscht werden,\n"
                "da noch Transaktion mit ihnen bestehen:\n\n" +
                failedDeletes.join('\n'));
        }

        resetForm();
    }

    if (!valid)
    {
        const QPoint globalPos = ui->categoryListView->mapFromGlobal(QPoint(0, 0));
        const int posX = -globalPos.x();
        const int posY = -globalPos.y();

        categoryListPopUp->setGeometry(posX + ui->categoryListView->width(),
            posY - ui->categoryListView->height() / 2 + 100,
            categoryListPopUp->width(),
            categoryListPopUp->height());

        categoryListPopUp->show();
    }
}

void CatView::on_categoryListView_clicked(const QModelIndex &index)
{
    categoryListPopUp->hide();
}
