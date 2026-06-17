#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database/databasemanager.h"

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

    setWindowTitle("Информационная система туристической компании");

    connect(ui->refreshButton, &QPushButton::clicked,
            this, &MainWindow::onRefreshButtonClicked);

    connect(ui->interestButton, &QPushButton::clicked,
            this, &MainWindow::onInterestButtonClicked);

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
        setWindowTitle("Панель администратора туристической компании");
    } else {
        ui->interestButton->setEnabled(true);
        ui->interestButton->setText("Заинтересовался");
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
    query.prepare(
        "INSERT INTO interested_packages (id_client, id_package) "
        "VALUES (:id_client, :id_package) "
        "ON CONFLICT (id_client, id_package) DO NOTHING"
        );

    query.bindValue(":id_client", m_clientId);
    query.bindValue(":id_package", packageId);

    if (!query.exec()) {
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