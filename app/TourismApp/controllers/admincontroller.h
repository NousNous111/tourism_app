#ifndef ADMINCONTROLLER_H
#define ADMINCONTROLLER_H

#include <QString>

class AdminController
{
public:
    AdminController();

    QString allClientsQuery() const;
    QString allOrdersQuery() const;
    QString allPackagesQuery() const;

    bool deletePackage(int packageId);

    QString lastError() const;

private:
    QString m_lastError;
};

#endif // ADMINCONTROLLER_H