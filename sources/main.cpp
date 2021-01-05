#include "adminregistrationview.h"
#include "menuview.h"
#include "addtransactionview.h"
#include <iostream>
#include <QApplication>
#include <QtSql>
#include "applicationview.h"
#include "dbmanager.h"
#include "inputcheck.h"
#include "global.h"

User g_currentUser = User();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DbManager db;
    db.createTables();

    for (int i = 1; i < argc; i++)
    {
        if (argv[i] == std::string("clean"))
        {
            db.deleteAllTables();

        }
        if (argv[i] == std::string("test"))
        {
            db.insertTestData();
        }
    }

    ApplicationView b;
    b.show();

    return a.exec();
}
