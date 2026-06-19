#ifndef ORDERCONTROLLER_H
#define ORDERCONTROLLER_H

#include <QString>
#include <QList>

class OrderController
{
public:
    OrderController();

    bool createOrder(int clientId, int packageId, double packagePrice);
    bool createOrderFromPackages(int clientId, const QList<int> &packageIds);

    QString clientOrdersQuery(int clientId) const;
    QString lastError() const;
    int lastCreatedOrderId() const;
    double lastDiscountPercent() const;
    double lastTotalCost() const;

private:
    QString sqlValue(const QString &value) const;
    QString packageIdsToSqlList(const QList<int> &packageIds) const;

private:
    QString m_lastError;
    int m_lastCreatedOrderId;
    double m_lastDiscountPercent;
    double m_lastTotalCost;
};

#endif // ORDERCONTROLLER_H