#include "ordercontroller.h"
#include "database/databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QStringList>
#include <QSet>

OrderController::OrderController()
    : m_lastError("")
    , m_lastCreatedOrderId(-1)
    , m_lastDiscountPercent(0.0)
    , m_lastTotalCost(0.0)
{
}

bool OrderController::createOrder(int clientId, int packageId, double packagePrice)
{
    Q_UNUSED(packagePrice);

    QList<int> packageIds;
    packageIds.append(packageId);

    return createOrderFromPackages(clientId, packageIds);
}

bool OrderController::createOrderFromPackages(int clientId, const QList<int> &packageIds)
{
    m_lastError.clear();
    m_lastCreatedOrderId = -1;
    m_lastDiscountPercent = 0.0;
    m_lastTotalCost = 0.0;

    if (clientId <= 0) {
        m_lastError = "Для текущего пользователя не найден клиентский профиль.";
        return false;
    }

    if (packageIds.isEmpty()) {
        m_lastError = "Не выбраны путевки для оформления заказа.";
        return false;
    }

    QString idsSql = packageIdsToSqlList(packageIds);

    QSqlDatabase database = DatabaseManager::instance().database();

    QSqlQuery packageQuery(database);
    QString packageSql =
        "SELECT "
        "tp.id_package, "
        "tp.base_price, "
        "c.country_name "
        "FROM travel_packages tp "
        "JOIN hotels h ON h.id_hotel = tp.id_hotel "
        "JOIN countries c ON c.id_country = h.id_country "
        "WHERE tp.id_package IN (" + idsSql + ")";

    if (!packageQuery.exec(packageSql)) {
        m_lastError = "Не удалось загрузить данные выбранных путевок:\n" +
                      packageQuery.lastError().text();
        return false;
    }

    double baseSum = 0.0;
    QSet<QString> countries;
    int foundPackagesCount = 0;

    while (packageQuery.next()) {
        baseSum += packageQuery.value("base_price").toDouble();
        countries.insert(packageQuery.value("country_name").toString());
        foundPackagesCount++;
    }

    packageQuery.finish();

    if (foundPackagesCount != packageIds.size()) {
        m_lastError = "Не все выбранные путевки найдены в базе данных.";
        return false;
    }

    double discountPercent = 0.0;

    if (packageIds.size() > 2) {
        discountPercent += 10.0;
    }

    if (baseSum >= 100000.0) {
        discountPercent += 5.0;
    }

    if (packageIds.size() >= 2 && countries.size() == 1) {
        discountPercent += 2.0;
    }

    double totalCost = baseSum * (1.0 - discountPercent / 100.0);

    QDate saleDate = QDate::currentDate();
    QDate departureDate = saleDate.addDays(14);

    if (!database.transaction()) {
        m_lastError = "Не удалось начать транзакцию:\n" + database.lastError().text();
        return false;
    }

    QSqlQuery insertOrderQuery(database);

    QString insertOrderSql =
        "INSERT INTO orders "
        "(id_client, sale_date, departure_date, total_cost, purchased_packages_count, discount_percent) "
        "VALUES (" +
        QString::number(clientId) + ", " +
        sqlValue(saleDate.toString("yyyy-MM-dd")) + ", " +
        sqlValue(departureDate.toString("yyyy-MM-dd")) + ", " +
        QString::number(totalCost, 'f', 2) + ", " +
        QString::number(packageIds.size()) + ", " +
        QString::number(discountPercent, 'f', 2) +
        ") "
        "RETURNING id_order";

    if (!insertOrderQuery.exec(insertOrderSql)) {
        database.rollback();
        m_lastError = "Не удалось создать заказ:\n" + insertOrderQuery.lastError().text();
        return false;
    }

    if (!insertOrderQuery.next()) {
        database.rollback();
        m_lastError = "Заказ создан, но его номер не получен.";
        return false;
    }

    int orderId = insertOrderQuery.value("id_order").toInt();
    insertOrderQuery.finish();

    for (int packageId : packageIds) {
        QSqlQuery insertPackageQuery(database);

        QString insertPackageSql =
            "INSERT INTO order_packages (id_order, id_package) "
            "VALUES (" +
            QString::number(orderId) + ", " +
            QString::number(packageId) +
            ")";

        if (!insertPackageQuery.exec(insertPackageSql)) {
            database.rollback();
            m_lastError = "Не удалось добавить путевку в заказ:\n" +
                          insertPackageQuery.lastError().text();
            return false;
        }

        insertPackageQuery.finish();
    }

    if (!database.commit()) {
        database.rollback();
        m_lastError = "Не удалось сохранить заказ:\n" + database.lastError().text();
        return false;
    }

    m_lastCreatedOrderId = orderId;
    m_lastDiscountPercent = discountPercent;
    m_lastTotalCost = totalCost;

    return true;
}

QString OrderController::clientOrdersQuery(int clientId) const
{
    return
        "SELECT "
        "o.id_order, "
        "o.sale_date, "
        "o.departure_date, "
        "STRING_AGG(h.hotel_name, ', ') AS hotels, "
        "STRING_AGG(DISTINCT c.country_name, ', ') AS countries, "
        "SUM(tp.duration_days) AS total_days, "
        "SUM(tp.base_price) AS base_sum, "
        "o.discount_percent, "
        "o.total_cost "
        "FROM orders o "
        "JOIN order_packages op ON op.id_order = o.id_order "
        "JOIN travel_packages tp ON tp.id_package = op.id_package "
        "JOIN hotels h ON h.id_hotel = tp.id_hotel "
        "JOIN countries c ON c.id_country = h.id_country "
        "WHERE o.id_client = " + QString::number(clientId) + " "
                                      "GROUP BY o.id_order, o.sale_date, o.departure_date, o.discount_percent, o.total_cost "
                                      "ORDER BY o.id_order DESC";
}

QString OrderController::lastError() const
{
    return m_lastError;
}

int OrderController::lastCreatedOrderId() const
{
    return m_lastCreatedOrderId;
}

double OrderController::lastDiscountPercent() const
{
    return m_lastDiscountPercent;
}

double OrderController::lastTotalCost() const
{
    return m_lastTotalCost;
}

QString OrderController::sqlValue(const QString &value) const
{
    QString escaped = value;
    escaped.replace("'", "''");
    return "'" + escaped + "'";
}

QString OrderController::packageIdsToSqlList(const QList<int> &packageIds) const
{
    QStringList ids;

    for (int packageId : packageIds) {
        ids.append(QString::number(packageId));
    }

    return ids.join(", ");
}