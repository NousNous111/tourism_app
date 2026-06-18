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

    bool addPackage(
        int hotelId,
        int durationDays,
        double basePrice,
        const QString &conditions
        );

    bool updatePackage(
        int packageId,
        int durationDays,
        double basePrice,
        const QString &conditions
        );

    bool deletePackage(int packageId);

    QString lastError() const;

private:
    QString sqlValue(const QString &value) const;

private:
    QString m_lastError;
};

#endif // ADMINCONTROLLER_H