#include "interestedpackageswindow.h"
#include "ui_interestedpackageswindow.h"

#include "database/databasemanager.h"

#include <QSqlQueryModel>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QAbstractItemView>

InterestedPackagesWindow::InterestedPackagesWindow(int clientId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InterestedPackagesWindow)
    , m_interestedModel(new QSqlQueryModel(this))
    , m_clientId(clientId)
{
    ui->setupUi(this);

    setWindowTitle("Мои интересующие путевки");

    connect(ui->closeButton, &QPushButton::clicked,
            this, &InterestedPackagesWindow::close);

    loadInterestedPackages();
}

InterestedPackagesWindow::~InterestedPackagesWindow()
{
    delete ui;
}

void InterestedPackagesWindow::loadInterestedPackages()
{
    if (m_clientId <= 0) {
        QMessageBox::warning(
            this,
            "Ошибка пользователя",
            "Для текущего пользователя не найден клиентский профиль."
            );
        return;
    }

    QString sql =
        "SELECT "
        "tp.id_package, "
        "h.hotel_name, "
        "c.country_name, "
        "tp.duration_days, "
        "tp.base_price, "
        "tp.conditions "
        "FROM interested_packages ip "
        "JOIN travel_packages tp ON tp.id_package = ip.id_package "
        "JOIN hotels h ON h.id_hotel = tp.id_hotel "
        "JOIN countries c ON c.id_country = h.id_country "
        "WHERE ip.id_client = " + QString::number(m_clientId) + " "
                                        "ORDER BY tp.id_package";

    m_interestedModel->setQuery(
        sql,
        DatabaseManager::instance().database()
        );

    if (m_interestedModel->lastError().isValid()) {
        QMessageBox::critical(
            this,
            "Ошибка загрузки данных",
            "Не удалось загрузить список интересующих путевок:\n" +
                m_interestedModel->lastError().text()
            );
        return;
    }

    m_interestedModel->setHeaderData(0, Qt::Horizontal, "ID");
    m_interestedModel->setHeaderData(1, Qt::Horizontal, "Отель");
    m_interestedModel->setHeaderData(2, Qt::Horizontal, "Страна");
    m_interestedModel->setHeaderData(3, Qt::Horizontal, "Дней");
    m_interestedModel->setHeaderData(4, Qt::Horizontal, "Стоимость");
    m_interestedModel->setHeaderData(5, Qt::Horizontal, "Условия");

    ui->interestedPackagesTableView->setModel(m_interestedModel);
    ui->interestedPackagesTableView->resizeColumnsToContents();
    ui->interestedPackagesTableView->horizontalHeader()->setStretchLastSection(true);
    ui->interestedPackagesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->interestedPackagesTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}