#include "myorderswindow.h"
#include "ui_myorderswindow.h"
#include "database/databasemanager.h"
#include "controllers/ordercontroller.h"

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
    OrderController orderController;

    m_ordersModel->setQuery(
        orderController.clientOrdersQuery(m_clientId),
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