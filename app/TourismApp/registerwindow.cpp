#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "database/databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>

RegisterWindow::RegisterWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);

    setWindowTitle("Регистрация клиента");

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    connect(ui->registerButton, &QPushButton::clicked,
            this, &RegisterWindow::onRegisterButtonClicked);

    connect(ui->cancelButton, &QPushButton::clicked,
            this, &RegisterWindow::onCancelButtonClicked);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

bool RegisterWindow::isPasswordValid(const QString &password) const
{
    QRegularExpression passwordRegex(
        "^(?=.*[A-Z])(?=.*\\d)(?=.*[^A-Za-z0-9]).{8,}$"
        );

    return passwordRegex.match(password).hasMatch();
}

QString RegisterWindow::hashPassword(const QString &password) const
{
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashBytes = QCryptographicHash::hash(
        passwordBytes,
        QCryptographicHash::Sha256
        );

    return QString(hashBytes.toHex());
}

static QString sqlValue(const QString &value)
{
    QString escaped = value;
    escaped.replace("'", "''");
    return "'" + escaped + "'";
}

void RegisterWindow::onRegisterButtonClicked()
{
    QString login = ui->loginLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();
    QString lastName = ui->lastNameLineEdit->text().trimmed();
    QString firstName = ui->firstNameLineEdit->text().trimmed();
    QString patronymic = ui->middleNameLineEdit->text().trimmed();
    QString address = ui->addressLineEdit->text().trimmed();
    QString phone = ui->phoneLineEdit->text().trimmed();

    if (login.isEmpty() || password.isEmpty() || lastName.isEmpty() ||
        firstName.isEmpty() || address.isEmpty() || phone.isEmpty()) {
        QMessageBox::warning(
            this,
            "Ошибка регистрации",
            "Заполните обязательные поля: логин, пароль, фамилия, имя, адрес и телефон."
            );
        return;
    }

    if (!isPasswordValid(password)) {
        QMessageBox::warning(
            this,
            "Ошибка регистрации",
            "Пароль должен быть не короче 8 символов и содержать хотя бы одну заглавную букву, одну цифру и один специальный символ."
            );
        return;
    }

    QSqlDatabase database = DatabaseManager::instance().database();

    if (!database.transaction()) {
        QMessageBox::critical(
            this,
            "Ошибка базы данных",
            "Не удалось начать транзакцию:\n" + database.lastError().text()
            );
        return;
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

        QMessageBox::critical(
            this,
            "Ошибка регистрации",
            "Не удалось проверить логин:\n" +
                checkQuery.lastError().text()
            );
        return;
    }

    if (checkQuery.next()) {
        database.rollback();

        QMessageBox::warning(
            this,
            "Ошибка регистрации",
            "Пользователь с таким логином уже существует."
            );
        return;
    }

    checkQuery.finish();

    QSqlQuery insertUserQuery(database);
    QString insertUserSql =
        "INSERT INTO users (login, password_hash, role) "
        "VALUES (" + loginSql + ", " + passwordHashSql + ", 'client')";

    if (!insertUserQuery.exec(insertUserSql)) {
        database.rollback();

        QMessageBox::critical(
            this,
            "Ошибка регистрации",
            "Не удалось создать пользователя:\n" +
                insertUserQuery.lastError().text()
            );
        return;
    }

    insertUserQuery.finish();

    QSqlQuery getUserIdQuery(database);
    QString getUserIdSql =
        "SELECT id_user "
        "FROM users "
        "WHERE login = " + loginSql;

    if (!getUserIdQuery.exec(getUserIdSql)) {
        database.rollback();

        QMessageBox::critical(
            this,
            "Ошибка регистрации",
            "Не удалось получить идентификатор пользователя:\n" +
                getUserIdQuery.lastError().text()
            );
        return;
    }

    if (!getUserIdQuery.next()) {
        database.rollback();

        QMessageBox::critical(
            this,
            "Ошибка регистрации",
            "Пользователь был создан, но его идентификатор не найден."
            );
        return;
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

        QMessageBox::critical(
            this,
            "Ошибка регистрации",
            "Не удалось создать профиль клиента:\n" +
                insertClientQuery.lastError().text()
            );
        return;
    }

    insertClientQuery.finish();

    if (!database.commit()) {
        database.rollback();

        QMessageBox::critical(
            this,
            "Ошибка базы данных",
            "Не удалось сохранить изменения:\n" +
                database.lastError().text()
            );
        return;
    }

    QMessageBox::information(
        this,
        "Регистрация завершена",
        "Пользователь успешно зарегистрирован. Теперь можно войти в систему."
        );

    accept();
}

void RegisterWindow::onCancelButtonClicked()
{
    reject();
}