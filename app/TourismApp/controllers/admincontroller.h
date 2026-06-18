#ifndef ADMINCONTROLLER_H
#define ADMINCONTROLLER_H

#include <QString>
#include <QStringList>

class AdminController
{
public:
    AdminController();

    QString allClientsQuery() const;
    QString allOrdersQuery() const;
    QString allPackagesQuery() const;

    QStringList hotelsForComboBox();

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