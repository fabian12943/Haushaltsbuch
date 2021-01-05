#include "applicationview.h"
#include "dbmanager.h"
#include "global.h"
#include <QApplication>

User g_currentUser = User();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize Database and create tables
    DbManager db;
    db.createTables();

    for (int i = 1; i < argc; i++)
    {
        // argv clean: Reset Database
        if (argv[i] == std::string("clean"))
        {
            db.deleteAllTables();
        }

        // argv test: Fill database with test data
        if (argv[i] == std::string("test"))
        {
            db.insertTestData();
        }
    }

    // Initialize and show Application UI
    ApplicationView b;
    b.show();

    return a.exec();
}
