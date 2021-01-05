#include "catview.h"
#include "ui_catview.h"
#include "dbmanager.h"
#include "global.h"

CatView::CatView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CatView)
    {
        ui->setupUi(this);

        // Initialize PopUpWidgets to display error message next to Inputline if necessary
        categoryPopUp = new PopUpWidget(this);
        categoryListPopUp = new PopUpWidget(this);
    }

CatView::~CatView()
{
    delete ui;
}

void CatView::resetForm()
{
    ui->categoryListView->setModel(DbManager::getCategoriesModel());
    ui->categoryLine->setText("");
    categoryPopUp->hide();
    categoryListPopUp->hide();
}

void CatView::on_BackButton_clicked()
{
    resetForm();
    emit Back();
}

void CatView::on_AddButton_clicked()
{
    // Save Userinput to Variable
    QString category = ui->categoryLine->text().trimmed();

    // userinput valid?
    bool valid = true;

    // Show error message next to the input line if Category is empty or already exists
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

    // Check if all input is valid
    if (valid)
    {
        // Create new category in the database with given input
        DbManager::addCategory(g_currentUser.getEmail(), category);

        // Reset input form to default
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

    // Prepares error message if no items were selected by the user
    if (list.isEmpty())
    {
        valid = false;
        categoryListPopUp->setPopupText("Keine Kategorien zum Löschen ausgewählt.");
    }
    else
    {
        // Takes all category names from IndexList that the user selected to delete and saves them in an array
        QStringList categoriesToDelete;
        foreach(const QModelIndex &index, list)
        {
            categoriesToDelete.append(index.data(Qt::DisplayRole).toString());
        }

        // Removes all categories in the array from database. Saves the names of categories that could not be
        // deleted in another array
        QStringList failedDeletes;
        for (const auto &category: categoriesToDelete)
        {
            bool success = DbManager::removeCategory(g_currentUser.getEmail(), category);
            if (!success) failedDeletes.append(category);
        }

        // Prepares error message with category names that could not be deleted successfully
        if (!failedDeletes.isEmpty())
        {
            valid = false;
            categoryListPopUp->setPopupText("Folgende Kategorien konnten nicht gelöscht werden,\n"
                "da noch Transaktion mit ihnen bestehen:\n\n" +
                failedDeletes.join('\n'));
        }

        // Resets Userinput Forms
        resetForm();
    }

    // Shows error message next to CategoryListView
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
