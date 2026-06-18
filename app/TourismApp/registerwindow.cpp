#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "controllers/clientcontroller.h"

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

void RegisterWindow::onRegisterButtonClicked()
{
    QString login = ui->loginLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();
    QString lastName = ui->lastNameLineEdit->text().trimmed();
    QString firstName = ui->firstNameLineEdit->text().trimmed();
    QString patronymic = ui->middleNameLineEdit->text().trimmed();
    QString address = ui->addressLineEdit->text().trimmed();
    QString phone = ui->phoneLineEdit->text().trimmed();

    ClientController clientController;

    bool success = clientController.registerClient(
        login,
        password,
        lastName,
        firstName,
        patronymic,
        address,
        phone
        );

    if (!success) {
        QMessageBox::warning(
            this,
            "Ошибка регистрации",
            clientController.lastError()
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