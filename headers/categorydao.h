#ifndef CATEGORYDAO_H
#define CATEGORYDAO_H

#include <QtSql>

class CategoryDAO
{
public:
    CategoryDAO();
    static bool addCategory(const QString& email, const QString& category);
    static bool removeCategory(const QString& email, const QString& category);
    static QSqlQueryModel* getCategoriesModel();
    static bool categoryExist(const QString& email, const QString& payoption);
};

#endif // CATEGORYDAO_H
