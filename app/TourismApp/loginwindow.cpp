#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "database/databasemanager.h"
#include "registerwindow.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
    , m_userId(-1)
    , m_clientId(-1)
{
    ui->setupUi(this);

    setWindowTitle("Авторизация");

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    connect(ui->loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginButtonClicked);

    connect(ui->registerButton, &QPushButton::clicked,
            this, &LoginWindow::onRegisterButtonClicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

int LoginWindow::userId() const
{
    return m_userId;
}

int LoginWindow::clientId() const
{
    return m_clientId;
}

QString LoginWindow::userRole() const
{
    return m_userRole;
}

QString LoginWindow::hashPassword(const QString &password) const
{
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashBytes = QCryptographicHash::hash(
        passwordBytes,
        QCryptographicHash::Sha256
        );

    return QString(hashBytes.toHex());
}

void LoginWindow::onLoginButtonClicked()
{
    QString login = ui->loginLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(
            this,
            "Ошибка входа",
            "Введите логин и пароль."
            );
        return;
    }

    QString passwordHash = hashPassword(password);

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT "
        "u.id_user, "
        "u.role, "
        "c.id_client "
        "FROM users u "
        "LEFT JOIN clients c ON c.id_user = u.id_user "
        "WHERE u.login = :login "
        "AND u.password_hash = :password_hash"
        );

    query.bindValue(":login", login);
    query.bindValue(":password_hash", passwordHash);

    if (!query.exec()) {
        QMessageBox::critical(
            this,
            "Ошибка базы данных",
            "Не удалось выполнить запрос авторизации:\n" +
                query.lastError().text()
            );
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(
            this,
            "Ошибка входа",
            "Неверный логин или пароль."
            );
        return;
    }

    m_userId = query.value("id_user").toInt();
    m_userRole = query.value("role").toString();

    if (!query.value("id_client").isNull()) {
        m_clientId = query.value("id_client").toInt();
    }

    accept();
}

void LoginWindow::onRegisterButtonClicked()
{
    RegisterWindow registerWindow(this);
    registerWindow.exec();
}