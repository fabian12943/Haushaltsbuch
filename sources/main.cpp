#include "applicationview.h"
#include "dbmanager.h"
#include "global.h"
#include <QApplication>

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
