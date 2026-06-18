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
#include <QModelIndex>

AdminWindow::AdminWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdminWindow)
    , m_clientsModel(new QSqlQueryModel(this))
    , m_ordersModel(new QSqlQueryModel(this))
    , m_packagesModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    setWindowTitle("Админ-панель");

    connect(ui->closeButton, &QPushButton::clicked,
            this, &AdminWindow::close);

    connect(ui->refreshButton, &QPushButton::clicked,
            this, &AdminWindow::onRefreshButtonClicked);

    connect(ui->deletePackageButton, &QPushButton::clicked,
            this, &AdminWindow::onDeletePackageButtonClicked);

    loadClients();
    loadOrders();
    loadPackages();
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

void AdminWindow::loadPackages()
{
    AdminController adminController;

    m_packagesModel->setQuery(
        adminController.allPackagesQuery(),
        DatabaseManager::instance().database()
        );

    if (m_packagesModel->lastError().isValid()) {
        QMessageBox::critical(
            this,
            "Ошибка загрузки путевок",
            "Не удалось загрузить список путевок:\n" +
                m_packagesModel->lastError().text()
            );
        return;
    }

    m_packagesModel->setHeaderData(0, Qt::Horizontal, "ID");
    m_packagesModel->setHeaderData(1, Qt::Horizontal, "Отель");
    m_packagesModel->setHeaderData(2, Qt::Horizontal, "Страна");
    m_packagesModel->setHeaderData(3, Qt::Horizontal, "Дней");
    m_packagesModel->setHeaderData(4, Qt::Horizontal, "Стоимость");
    m_packagesModel->setHeaderData(5, Qt::Horizontal, "Условия");

    ui->packagesTableView->setModel(m_packagesModel);
    ui->packagesTableView->resizeColumnsToContents();
    ui->packagesTableView->horizontalHeader()->setStretchLastSection(true);
    ui->packagesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->packagesTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AdminWindow::onRefreshButtonClicked()
{
    loadClients();
    loadOrders();
    loadPackages();

    QMessageBox::information(
        this,
        "Обновление",
        "Данные успешно обновлены."
        );
}

void AdminWindow::onDeletePackageButtonClicked()
{
    QModelIndex currentIndex = ui->packagesTableView->currentIndex();

    if (!currentIndex.isValid()) {
        QMessageBox::warning(
            this,
            "Выбор путевки",
            "Сначала выберите путевку в таблице."
            );
        return;
    }

    int row = currentIndex.row();
    int packageId = m_packagesModel->data(m_packagesModel->index(row, 0)).toInt();

    int answer = QMessageBox::question(
        this,
        "Удаление путевки",
        "Вы действительно хотите удалить выбранную путевку?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (answer != QMessageBox::Yes) {
        return;
    }

    AdminController adminController;

    if (!adminController.deletePackage(packageId)) {
        QMessageBox::warning(
            this,
            "Ошибка удаления",
            adminController.lastError()
            );
        return;
    }

    QMessageBox::information(
        this,
        "Удаление выполнено",
        "Путевка успешно удалена."
        );

    loadPackages();
}