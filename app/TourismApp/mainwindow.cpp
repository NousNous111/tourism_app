#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database/databasemanager.h"

#include <QSqlQueryModel>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_packagesModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    setWindowTitle("Информационная система туристической компании");

    loadTravelPackages();
}

MainWindow::~MainWindow()
{
    delete ui;
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