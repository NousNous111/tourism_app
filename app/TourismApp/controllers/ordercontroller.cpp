#include "ordercontroller.h"
#include "database/databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

OrderController::OrderController()
    : m_lastError("")
    , m_lastCreatedOrderId(-1)
{
}

bool OrderController::createOrder(int clientId, int packageId, double packagePrice)
{
    m_lastError.clear();
    m_lastCreatedOrderId = -1;

    if (clientId <= 0) {
        m_lastError = "Для текущего пользователя не найден клиентский профиль.";
        return false;
    }

    if (packageId <= 0) {
        m_lastError = "Не выбрана путевка для оформления заказа.";
        return false;
    }

    QDate saleDate = QDate::currentDate();
    QDate departureDate = saleDate.addDays(14);

    QSqlDatabase database = DatabaseManager::instance().database();

    if (!database.transaction()) {
        m_lastError = "Не удалось начать транзакцию:\n" + database.lastError().text();
        return false;
    }

    QSqlQuery insertOrderQuery(database);

    QString insertOrderSql =
        "INSERT INTO orders "
        "(id_client, sale_date, departure_date, total_cost, purchased_packages_count) "
        "VALUES (" +
        QString::number(clientId) + ", " +
        sqlValue(saleDate.toString("yyyy-MM-dd")) + ", " +
        sqlValue(departureDate.toString("yyyy-MM-dd")) + ", " +
        QString::number(packagePrice, 'f', 2) + ", "
                                                "1"
                                                ")";

    if (!insertOrderQuery.exec(insertOrderSql)) {
        database.rollback();
        m_lastError = "Не удалось создать заказ:\n" + insertOrderQuery.lastError().text();
        return false;
    }

    insertOrderQuery.finish();

    QSqlQuery getOrderIdQuery(database);

    QString getOrderIdSql =
        "SELECT id_order "
        "FROM orders "
        "WHERE id_client = " + QString::number(clientId) + " "
                                      "ORDER BY id_order DESC "
                                      "LIMIT 1";

    if (!getOrderIdQuery.exec(getOrderIdSql)) {
        database.rollback();
        m_lastError = "Не удалось получить номер заказа:\n" + getOrderIdQuery.lastError().text();
        return false;
    }

    if (!getOrderIdQuery.next()) {
        database.rollback();
        m_lastError = "Заказ создан, но его номер не найден.";
        return false;
    }

    int orderId = getOrderIdQuery.value("id_order").toInt();
    getOrderIdQuery.finish();

    QSqlQuery insertOrderPackageQuery(database);

    QString insertOrderPackageSql =
        "INSERT INTO order_packages (id_order, id_package) "
        "VALUES (" +
        QString::number(orderId) + ", " +
        QString::number(packageId) +
        ")";

    if (!insertOrderPackageQuery.exec(insertOrderPackageSql)) {
        database.rollback();
        m_lastError = "Не удалось добавить путевку в заказ:\n" +
                      insertOrderPackageQuery.lastError().text();
        return false;
    }

    insertOrderPackageQuery.finish();

    if (!database.commit()) {
        database.rollback();
        m_lastError = "Не удалось сохранить заказ:\n" + database.lastError().text();
        return false;
    }

    m_lastCreatedOrderId = orderId;
    return true;
}

QString OrderController::clientOrdersQuery(int clientId) const
{
    return
        "SELECT "
        "o.id_order, "
        "o.sale_date, "
        "o.departure_date, "
        "h.hotel_name, "
        "c.country_name, "
        "tp.duration_days, "
        "tp.base_price, "
        "o.total_cost "
        "FROM orders o "
        "JOIN order_packages op ON op.id_order = o.id_order "
        "JOIN travel_packages tp ON tp.id_package = op.id_package "
        "JOIN hotels h ON h.id_hotel = tp.id_hotel "
        "JOIN countries c ON c.id_country = h.id_country "
        "WHERE o.id_client = " + QString::number(clientId) + " "
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

QString OrderController::sqlValue(const QString &value) const
{
    QString escaped = value;
    escaped.replace("'", "''");
    return "'" + escaped + "'";
}