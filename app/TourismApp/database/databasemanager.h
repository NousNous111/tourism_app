#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool connectToDatabase();
    void closeConnection();

    QSqlDatabase database() const;
    QString lastError() const;

private:
    DatabaseManager();
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

private:
    QSqlDatabase m_database;
    QString m_lastError;
};

#endif // DATABASEMANAGER_H