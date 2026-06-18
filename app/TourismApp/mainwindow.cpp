#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database/databasemanager.h"
#include "myorderswindow.h"
#include "controllers/ordercontroller.h"

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QPushButton>

MainWindow::MainWindow(int userId, int clientId, const QString &userRole, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_packagesModel(new QSqlQueryModel(this))
    , m_userId(userId)
    , m_clientId(clientId)
    , m_userRole(userRole)
{
    ui->setupUi(this);

    connect(ui->refreshButton, &QPushButton::clicked,
            this, &MainWindow::onRefreshButtonClicked);

    connect(ui->interestButton, &QPushButton::clicked,
            this, &MainWindow::onInterestButtonClicked);

    connect(ui->orderButton, &QPushButton::clicked,
            this, &MainWindow::onOrderButtonClicked);

    connect(ui->myOrdersButton, &QPushButton::clicked,
            this, &MainWindow::onMyOrdersButtonClicked);

    configureInterfaceByRole();
    loadTravelPackages();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configureInterfaceByRole()
{
    if (m_userRole == "admin") {
        ui->interestButton->setEnabled(false);
        ui->interestButton->setText("Недоступно для администратора");

        ui->orderButton->setEnabled(false);
        ui->orderButton->setText("Заказ недоступен");

        ui->myOrdersButton->setEnabled(false);
        ui->myOrdersButton->setText("Заказы клиента недоступны");

        setWindowTitle("Панель администратора туристической компании");
    } else {
        ui->interestButton->setEnabled(true);
        ui->interestButton->setText("Заинтересовался");

        ui->orderButton->setEnabled(true);
        ui->orderButton->setText("Оформить заказ");

        ui->myOrdersButton->setEnabled(true);
        ui->myOrdersButton->setText("Мои заказы");

        setWindowTitle("Кабинет клиента туристической компании");
    }
}

void MainWindow::loadTravelPackages()
{
    m_packagesModel->setQuery(
        "SELECT "
        "tp.id_package, "
        "h.hotel_name, "
        "c.country_name, "
        "tp.duration_days, "
        "tp.base_price, "
        "tp.conditions "
        "FROM travel_packages tp "
        "JOIN hotels h ON tp.id_hotel = h.id_hotel "
        "JOIN countries c ON h.id_country = c.id_country "
        "ORDER BY tp.id_package",
        DatabaseManager::instance().database()
        );

    if (m_packagesModel->lastError().isValid()) {
        QMessageBox::critical(
            this,
            "Ошибка загрузки данных",
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

void MainWindow::onRefreshButtonClicked()
{
    loadTravelPackages();
}

void MainWindow::onInterestButtonClicked()
{
    if (m_userRole == "admin") {
        QMessageBox::warning(
            this,
            "Ограничение доступа",
            "Администратор не может добавлять путевки в список интересующих."
            );
        return;
    }

    if (m_clientId <= 0) {
        QMessageBox::warning(
            this,
            "Ошибка пользователя",
            "Для текущего пользователя не найден клиентский профиль."
            );
        return;
    }

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

    QSqlQuery query(DatabaseManager::instance().database());

    QString sql =
        "INSERT INTO interested_packages (id_client, id_package) "
        "VALUES (" +
        QString::number(m_clientId) + ", " +
        QString::number(packageId) +
        ") "
        "ON CONFLICT (id_client, id_package) DO NOTHING";

    if (!query.exec(sql)) {
        QMessageBox::critical(
            this,
            "Ошибка",
            "Не удалось добавить путевку в список интересующих:\n" +
                query.lastError().text()
            );
        return;
    }

    QMessageBox::information(
        this,
        "Готово",
        "Путевка добавлена в список интересующих."
        );
}

void MainWindow::onOrderButtonClicked()
{
    if (m_userRole == "admin") {
        QMessageBox::warning(
            this,
            "Ограничение доступа",
            "Администратор не может оформлять заказы."
            );
        return;
    }

    if (m_clientId <= 0) {
        QMessageBox::warning(
            this,
            "Ошибка пользователя",
            "Для текущего пользователя не найден клиентский профиль."
            );
        return;
    }

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
    double packagePrice = m_packagesModel->data(m_packagesModel->index(row, 4)).toDouble();

    OrderController orderController;

    if (!orderController.createOrder(m_clientId, packageId, packagePrice)) {
        QMessageBox::critical(
            this,
            "Ошибка оформления заказа",
            orderController.lastError()
            );
        return;
    }

    QMessageBox::information(
        this,
        "Заказ оформлен",
        "Заказ успешно оформлен.\nНомер заказа: " +
            QString::number(orderController.lastCreatedOrderId())
        );
}

void MainWindow::onMyOrdersButtonClicked()
{
    if (m_userRole == "admin") {
        QMessageBox::warning(
            this,
            "Ограничение доступа",
            "Администратор не может просматривать клиентский список заказов."
            );
        return;
    }

    if (m_clientId <= 0) {
        QMessageBox::warning(
            this,
            "Ошибка пользователя",
            "Для текущего пользователя не найден клиентский профиль."
            );
        return;
    }

    MyOrdersWindow myOrdersWindow(m_clientId, this);
    myOrdersWindow.exec();
}