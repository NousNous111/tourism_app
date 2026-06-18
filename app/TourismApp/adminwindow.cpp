#include "adminwindow.h"
#include "ui_adminwindow.h"

#include "database/databasemanager.h"
#include "controllers/admincontroller.h"

#include <QSqlQueryModel>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QAbstractItemView>

AdminWindow::AdminWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdminWindow)
    , m_clientsModel(new QSqlQueryModel(this))
    , m_ordersModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    setWindowTitle("Админ-панель");

    connect(ui->closeButton, &QPushButton::clicked,
            this, &AdminWindow::close);

    loadClients();
    loadOrders();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::loadClients()
{
    AdminController adminController;

    m_clientsModel->setQuery(
        adminController.allClientsQuery(),
        DatabaseManager::instance().database()
        );

    if (m_clientsModel->lastError().isValid()) {
        QMessageBox::critical(
            this,
            "Ошибка загрузки клиентов",
            "Не удалось загрузить список клиентов:\n" +
                m_clientsModel->lastError().text()
            );
        return;
    }

    m_clientsModel->setHeaderData(0, Qt::Horizontal, "ID клиента");
    m_clientsModel->setHeaderData(1, Qt::Horizontal, "Логин");
    m_clientsModel->setHeaderData(2, Qt::Horizontal, "Фамилия");
    m_clientsModel->setHeaderData(3, Qt::Horizontal, "Имя");
    m_clientsModel->setHeaderData(4, Qt::Horizontal, "Отчество");
    m_clientsModel->setHeaderData(5, Qt::Horizontal, "Адрес");
    m_clientsModel->setHeaderData(6, Qt::Horizontal, "Телефон");

    ui->clientsTableView->setModel(m_clientsModel);
    ui->clientsTableView->resizeColumnsToContents();
    ui->clientsTableView->horizontalHeader()->setStretchLastSection(true);
    ui->clientsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->clientsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AdminWindow::loadOrders()
{
    AdminController adminController;

    m_ordersModel->setQuery(
        adminController.allOrdersQuery(),
        DatabaseManager::instance().database()
        );

    if (m_ordersModel->lastError().isValid()) {
        QMessageBox::critical(
            this,
            "Ошибка загрузки заказов",
            "Не удалось загрузить список заказов:\n" +
                m_ordersModel->lastError().text()
            );
        return;
    }

    m_ordersModel->setHeaderData(0, Qt::Horizontal, "№ заказа");
    m_ordersModel->setHeaderData(1, Qt::Horizontal, "ID клиента");
    m_ordersModel->setHeaderData(2, Qt::Horizontal, "Фамилия");
    m_ordersModel->setHeaderData(3, Qt::Horizontal, "Имя");
    m_ordersModel->setHeaderData(4, Qt::Horizontal, "Дата продажи");
    m_ordersModel->setHeaderData(5, Qt::Horizontal, "Дата выезда");
    m_ordersModel->setHeaderData(6, Qt::Horizontal, "Отель");
    m_ordersModel->setHeaderData(7, Qt::Horizontal, "Страна");
    m_ordersModel->setHeaderData(8, Qt::Horizontal, "Дней");
    m_ordersModel->setHeaderData(9, Qt::Horizontal, "Сумма");

    ui->ordersTableView->setModel(m_ordersModel);
    ui->ordersTableView->resizeColumnsToContents();
    ui->ordersTableView->horizontalHeader()->setStretchLastSection(true);
    ui->ordersTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ordersTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}