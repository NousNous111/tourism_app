#ifndef ORDERCONTROLLER_H
#define ORDERCONTROLLER_H

#include <QString>

class OrderController
{
public:
    OrderController();

    bool createOrder(int clientId, int packageId, double packagePrice);

    QString clientOrdersQuery(int clientId) const;
    QString lastError() const;
    int lastCreatedOrderId() const;

private:
    QString sqlValue(const QString &value) const;

private:
    QString m_lastError;
    int m_lastCreatedOrderId;
};

#endif // ORDERCONTROLLER_H