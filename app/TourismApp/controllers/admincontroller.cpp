#include "admincontroller.h"

AdminController::AdminController()
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