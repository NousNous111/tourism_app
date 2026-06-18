#ifndef ADMINCONTROLLER_H
#define ADMINCONTROLLER_H

#include <QString>

class AdminController
{
public:
    AdminController();

    QString allClientsQuery() const;
    QString allOrdersQuery() const;
};

#endif // ADMINCONTROLLER_H