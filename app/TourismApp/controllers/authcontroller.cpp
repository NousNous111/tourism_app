#include "authcontroller.h"
#include "database/databasemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>

AuthController::AuthController()
    : m_currentUser()
    , m_currentClientId(-1)
    , m_lastError("")
{
}

bool AuthController::login(const QString &login, const QString &password)
{
    m_currentUser = User();
    m_currentClientId = -1;
    m_lastError.clear();

    QString passwordHash = hashPassword(password);

    QSqlQuery query(DatabaseManager::instance().database());

    QString escapedLogin = login;
    escapedLogin.replace("'", "''");

    QString escapedPasswordHash = passwordHash;
    escapedPasswordHash.replace("'", "''");

    QString sql =
        "SELECT "
        "u.id_user, "
        "u.login, "
        "u.role, "
        "c.id_client "
        "FROM users u "
        "LEFT JOIN clients c ON c.id_user = u.id_user "
        "WHERE u.login = '" + escapedLogin + "' "
                         "AND u.password_hash = '" + escapedPasswordHash + "'";

    if (!query.exec(sql)) {
        m_lastError = "Не удалось выполнить запрос авторизации:\n" +
                      query.lastError().text();
        return false;
    }

    if (!query.next()) {
        m_lastError = "Неверный логин или пароль.";
        return false;
    }

    int idUser = query.value("id_user").toInt();
    QString userLogin = query.value("login").toString();
    QString role = query.value("role").toString();

    m_currentUser = User(idUser, userLogin, role);

    if (!query.value("id_client").isNull()) {
        m_currentClientId = query.value("id_client").toInt();
    }

    return true;
}

User AuthController::currentUser() const
{
    return m_currentUser;
}

int AuthController::currentClientId() const
{
    return m_currentClientId;
}

QString AuthController::lastError() const
{
    return m_lastError;
}

QString AuthController::hashPassword(const QString &password) const
{
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashBytes = QCryptographicHash::hash(
        passwordBytes,
        QCryptographicHash::Sha256
        );

    return QString(hashBytes.toHex());
}