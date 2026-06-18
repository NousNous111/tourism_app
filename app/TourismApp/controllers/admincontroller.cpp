#include "admincontroller.h"
#include "database/databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

AdminController::AdminController()
    : m_lastError("")
{
}

QString AdminController::allClientsQuery() const
{
    return
        "SELECT "
        "c.id_client, "
        "u.login, "
        "c.last_name, "
        "c.first_name, "
        "c.patronymic, "
        "c.address, "
        "c.phone "
        "FROM clients c "
        "JOIN users u ON u.id_user = c.id_user "
        "ORDER BY c.id_client";
}

QString AdminController::allOrdersQuery() const
{
    return
        "SELECT "
        "o.id_order, "
        "cl.id_client, "
        "cl.last_name, "
        "cl.first_name, "
        "o.sale_date, "
        "o.departure_date, "
        "h.hotel_name, "
        "co.country_name, "
        "tp.duration_days, "
        "o.total_cost "
        "FROM orders o "
        "JOIN clients cl ON cl.id_client = o.id_client "
        "JOIN order_packages op ON op.id_order = o.id_order "
        "JOIN travel_packages tp ON tp.id_package = op.id_package "
        "JOIN hotels h ON h.id_hotel = tp.id_hotel "
        "JOIN countries co ON co.id_country = h.id_country "
        "ORDER BY o.id_order DESC";
}

QString AdminController::allPackagesQuery() const
{
    return
        "SELECT "
        "tp.id_package, "
        "h.hotel_name, "
        "co.country_name, "
        "tp.duration_days, "
        "tp.base_price, "
        "tp.conditions "
        "FROM travel_packages tp "
        "JOIN hotels h ON h.id_hotel = tp.id_hotel "
        "JOIN countries co ON co.id_country = h.id_country "
        "ORDER BY tp.id_package";
}

bool AdminController::deletePackage(int packageId)
{
    m_lastError.clear();

    if (packageId <= 0) {
        m_lastError = "Некорректный идентификатор путевки.";
        return false;
    }

    QSqlDatabase database = DatabaseManager::instance().database();

    if (!database.transaction()) {
        m_lastError = "Не удалось начать транзакцию:\n" + database.lastError().text();
        return false;
    }

    QSqlQuery deleteInterestedQuery(database);
    QString deleteInterestedSql =
        "DELETE FROM interested_packages "
        "WHERE id_package = " + QString::number(packageId);

    if (!deleteInterestedQuery.exec(deleteInterestedSql)) {
        database.rollback();
        m_lastError = "Не удалось удалить путевку из списка интересующих:\n" +
                      deleteInterestedQuery.lastError().text();
        return false;
    }

    deleteInterestedQuery.finish();

    QSqlQuery checkOrdersQuery(database);
    QString checkOrdersSql =
        "SELECT id_order "
        "FROM order_packages "
        "WHERE id_package = " + QString::number(packageId) + " "
                                       "LIMIT 1";

    if (!checkOrdersQuery.exec(checkOrdersSql)) {
        database.rollback();
        m_lastError = "Не удалось проверить наличие заказов по путевке:\n" +
                      checkOrdersQuery.lastError().text();
        return false;
    }

    if (checkOrdersQuery.next()) {
        database.rollback();
        m_lastError = "Нельзя удалить путевку, потому что она уже используется в заказах.";
        return false;
    }

    checkOrdersQuery.finish();

    QSqlQuery deletePackageQuery(database);
    QString deletePackageSql =
        "DELETE FROM travel_packages "
        "WHERE id_package = " + QString::number(packageId);

    if (!deletePackageQuery.exec(deletePackageSql)) {
        database.rollback();
        m_lastError = "Не удалось удалить путевку:\n" +
                      deletePackageQuery.lastError().text();
        return false;
    }

    deletePackageQuery.finish();

    if (!database.commit()) {
        database.rollback();
        m_lastError = "Не удалось сохранить изменения:\n" + database.lastError().text();
        return false;
    }

    return true;
}

QString AdminController::lastError() const
{
    return m_lastError;
}