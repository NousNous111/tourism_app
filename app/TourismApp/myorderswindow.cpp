#include "myorderswindow.h"
#include "ui_myorderswindow.h"
#include "database/databasemanager.h"

#include <QSqlQueryModel>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QAbstractItemView>

MyOrdersWindow::MyOrdersWindow(int clientId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MyOrdersWindow)
    , m_ordersModel(new QSqlQueryModel(this))
    , m_clientId(clientId)
{
    ui->setupUi(this);

    setWindowTitle("Мои заказы");

    connect(ui->closeButton, &QPushButton::clicked,
            this, &MyOrdersWindow::close);

    loadOrders();
}

MyOrdersWindow::~MyOrdersWindow()
{
    delete ui;
}

void MyOrdersWindow::loadOrders()
{
    m_ordersModel->setQuery(
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
        "WHERE o.id_client = " + QString::number(m_clientId) + " "
                                            "ORDER BY o.id_order DESC",
        DatabaseManager::instance().database()
        );

    if (m_ordersModel->lastError().isValid()) {
        QMessageBox::critical(
            this,
            "Ошибка загрузки данных",
            "Не удалось загрузить список заказов:\n" +
                m_ordersModel->lastError().text()
            );
        return;
    }

    m_ordersModel->setHeaderData(0, Qt::Horizontal, "№ заказа");
    m_ordersModel->setHeaderData(1, Qt::Horizontal, "Дата продажи");
    m_ordersModel->setHeaderData(2, Qt::Horizontal, "Дата выезда");
    m_ordersModel->setHeaderData(3, Qt::Horizontal, "Отель");
    m_ordersModel->setHeaderData(4, Qt::Horizontal, "Страна");
    m_ordersModel->setHeaderData(5, Qt::Horizontal, "Дней");
    m_ordersModel->setHeaderData(6, Qt::Horizontal, "Цена путевки");
    m_ordersModel->setHeaderData(7, Qt::Horizontal, "Итого");

    ui->ordersTableView->setModel(m_ordersModel);
    ui->ordersTableView->resizeColumnsToContents();
    ui->ordersTableView->horizontalHeader()->setStretchLastSection(true);
    ui->ordersTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ordersTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}