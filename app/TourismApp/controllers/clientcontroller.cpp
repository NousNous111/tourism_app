#include "clientcontroller.h"
#include "database/databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QRegularExpression>

ClientController::ClientController()
    : m_lastError("")
{
}

bool ClientController::registerClient(
    const QString &login,
    const QString &password,
    const QString &lastName,
    const QString &firstName,
    const QString &patronymic,
    const QString &address,
    const QString &phone
    )
{
    m_lastError.clear();

    if (login.isEmpty() || password.isEmpty() || lastName.isEmpty() ||
        firstName.isEmpty() || address.isEmpty() || phone.isEmpty()) {
        m_lastError = "Заполните обязательные поля: логин, пароль, фамилия, имя, адрес и телефон.";
        return false;
    }

    if (!isPasswordValid(password)) {
        m_lastError = "Пароль должен быть не короче 8 символов и содержать хотя бы одну заглавную букву, одну цифру и один специальный символ.";
        return false;
    }

    QSqlDatabase database = DatabaseManager::instance().database();

    if (!database.transaction()) {
        m_lastError = "Не удалось начать транзакцию:\n" + database.lastError().text();
        return false;
    }

    QString loginSql = sqlValue(login);
    QString passwordHashSql = sqlValue(hashPassword(password));
    QString lastNameSql = sqlValue(lastName);
    QString firstNameSql = sqlValue(firstName);
    QString patronymicSql = sqlValue(patronymic);
    QString addressSql = sqlValue(address);
    QString phoneSql = sqlValue(phone);

    QSqlQuery checkQuery(database);
    QString checkSql =
        "SELECT id_user "
        "FROM users "
        "WHERE login = " + loginSql;

    if (!checkQuery.exec(checkSql)) {
        database.rollback();
        m_lastError = "Не удалось проверить логин:\n" + checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next()) {
        database.rollback();
        m_lastError = "Пользователь с таким логином уже существует.";
        return false;
    }

    checkQuery.finish();

    QSqlQuery insertUserQuery(database);
    QString insertUserSql =
        "INSERT INTO users (login, password_hash, role) "
        "VALUES (" + loginSql + ", " + passwordHashSql + ", 'client')";

    if (!insertUserQuery.exec(insertUserSql)) {
        database.rollback();
        m_lastError = "Не удалось создать пользователя:\n" + insertUserQuery.lastError().text();
        return false;
    }

    insertUserQuery.finish();

    QSqlQuery getUserIdQuery(database);
    QString getUserIdSql =
        "SELECT id_user "
        "FROM users "
        "WHERE login = " + loginSql;

    if (!getUserIdQuery.exec(getUserIdSql)) {
        database.rollback();
        m_lastError = "Не удалось получить идентификатор пользователя:\n" + getUserIdQuery.lastError().text();
        return false;
    }

    if (!getUserIdQuery.next()) {
        database.rollback();
        m_lastError = "Пользователь был создан, но его идентификатор не найден.";
        return false;
    }

    int userId = getUserIdQuery.value("id_user").toInt();
    getUserIdQuery.finish();

    QSqlQuery insertClientQuery(database);
    QString insertClientSql =
        "INSERT INTO clients "
        "(id_user, last_name, first_name, patronymic, address, phone) "
        "VALUES (" +
        QString::number(userId) + ", " +
        lastNameSql + ", " +
        firstNameSql + ", " +
        patronymicSql + ", " +
        addressSql + ", " +
        phoneSql +
        ")";

    if (!insertClientQuery.exec(insertClientSql)) {
        database.rollback();
        m_lastError = "Не удалось создать профиль клиента:\n" + insertClientQuery.lastError().text();
        return false;
    }

    insertClientQuery.finish();

    if (!database.commit()) {
        database.rollback();
        m_lastError = "Не удалось сохранить изменения:\n" + database.lastError().text();
        return false;
    }

    return true;
}

bool ClientController::loadClientProfile(
    int clientId,
    QString &lastName,
    QString &firstName,
    QString &patronymic,
    QString &address,
    QString &phone
    )
{
    m_lastError.clear();

    if (clientId <= 0) {
        m_lastError = "Некорректный идентификатор клиента.";
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().database());

    QString sql =
        "SELECT last_name, first_name, patronymic, address, phone "
        "FROM clients "
        "WHERE id_client = " + QString::number(clientId);

    if (!query.exec(sql)) {
        m_lastError = "Не удалось загрузить профиль клиента:\n" +
                      query.lastError().text();
        return false;
    }

    if (!query.next()) {
        m_lastError = "Профиль клиента не найден.";
        return false;
    }

    lastName = query.value("last_name").toString();
    firstName = query.value("first_name").toString();
    patronymic = query.value("patronymic").toString();
    address = query.value("address").toString();
    phone = query.value("phone").toString();

    return true;
}

bool ClientController::updateClientProfile(
    int clientId,
    const QString &lastName,
    const QString &firstName,
    const QString &patronymic,
    const QString &address,
    const QString &phone
    )
{
    m_lastError.clear();

    if (clientId <= 0) {
        m_lastError = "Некорректный идентификатор клиента.";
        return false;
    }

    if (lastName.isEmpty() || firstName.isEmpty() ||
        address.isEmpty() || phone.isEmpty()) {
        m_lastError = "Заполните обязательные поля: фамилия, имя, адрес и телефон.";
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().database());

    QString sql =
        "UPDATE clients SET "
        "last_name = " + sqlValue(lastName) + ", "
                               "first_name = " + sqlValue(firstName) + ", "
                                "patronymic = " + sqlValue(patronymic) + ", "
                                 "address = " + sqlValue(address) + ", "
                              "phone = " + sqlValue(phone) + " "
                            "WHERE id_client = " + QString::number(clientId);

    if (!query.exec(sql)) {
        m_lastError = "Не удалось обновить профиль клиента:\n" +
                      query.lastError().text();
        return false;
    }

    return true;
}

QString ClientController::lastError() const
{
    return m_lastError;
}

bool ClientController::isPasswordValid(const QString &password) const
{
    QRegularExpression passwordRegex(
        "^(?=.*[A-Z])(?=.*\\d)(?=.*[^A-Za-z0-9]).{8,}$"
        );

    return passwordRegex.match(password).hasMatch();
}

QString ClientController::hashPassword(const QString &password) const
{
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashBytes = QCryptographicHash::hash(
        passwordBytes,
        QCryptographicHash::Sha256
        );

    return QString(hashBytes.toHex());
}

QString ClientController::sqlValue(const QString &value) const
{
    QString escaped = value;
    escaped.replace("'", "''");
    return "'" + escaped + "'";
}