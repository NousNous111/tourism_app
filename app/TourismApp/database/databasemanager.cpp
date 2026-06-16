#include "databasemanager.h"

#include <QSqlError>
#include <QDebug>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
    closeConnection();
}

bool DatabaseManager::connectToDatabase()
{
    if (QSqlDatabase::contains("main_connection")) {
        m_database = QSqlDatabase::database("main_connection");
    } else {
        m_database = QSqlDatabase::addDatabase("QPSQL", "main_connection");
    }

    m_database.setHostName("localhost");
    m_database.setPort(5432);
    m_database.setDatabaseName("tourism_app");
    m_database.setUserName("daniil");
    m_database.setPassword("");

    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        qDebug() << "Database connection error:" << m_lastError;
        return false;
    }

    m_lastError.clear();
    qDebug() << "Database connection established";
    return true;
}

void DatabaseManager::closeConnection()
{
    if (m_database.isValid() && m_database.isOpen()) {
        m_database.close();
    }
}

QSqlDatabase DatabaseManager::database() const
{
    return m_database;
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}