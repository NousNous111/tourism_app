#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database/databasemanager.h"

#include <QSqlQueryModel>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QAbstractItemView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_hotelsModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    setWindowTitle("Информационная система туристической компании");

    loadHotels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadHotels()
{
    m_hotelsModel->setQuery(
        "SELECT "
        "h.id_hotel, "
        "h.hotel_name, "
        "c.country_name, "
        "h.hotel_class, "
        "h.hotel_address "
        "FROM hotels h "
        "JOIN countries c ON h.id_country = c.id_country "
        "ORDER BY h.id_hotel",
        DatabaseManager::instance().database()
        );

    if (m_hotelsModel->lastError().isValid()) {
        QMessageBox::critical(
            this,
            "Ошибка загрузки данных",
            "Не удалось загрузить список отелей:\n" +
                m_hotelsModel->lastError().text()
            );
        return;
    }

    m_hotelsModel->setHeaderData(0, Qt::Horizontal, "ID");
    m_hotelsModel->setHeaderData(1, Qt::Horizontal, "Название отеля");
    m_hotelsModel->setHeaderData(2, Qt::Horizontal, "Страна");
    m_hotelsModel->setHeaderData(3, Qt::Horizontal, "Класс");
    m_hotelsModel->setHeaderData(4, Qt::Horizontal, "Адрес");

    ui->hotelsTableView->setModel(m_hotelsModel);
    ui->hotelsTableView->resizeColumnsToContents();
    ui->hotelsTableView->horizontalHeader()->setStretchLastSection(true);
    ui->hotelsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->hotelsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}