#include "profilewindow.h"
#include "ui_profilewindow.h"
#include "controllers/clientcontroller.h"

#include <QMessageBox>
#include <QPushButton>

ProfileWindow::ProfileWindow(int clientId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileWindow)
    , m_clientId(clientId)
{
    ui->setupUi(this);

    setWindowTitle("Мой профиль");

    connect(ui->saveButton, &QPushButton::clicked,
            this, &ProfileWindow::onSaveButtonClicked);

    connect(ui->closeButton, &QPushButton::clicked,
            this, &ProfileWindow::onCloseButtonClicked);

    loadProfile();
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

void ProfileWindow::loadProfile()
{
    QString lastName;
    QString firstName;
    QString patronymic;
    QString address;
    QString phone;

    ClientController clientController;

    if (!clientController.loadClientProfile(
            m_clientId,
            lastName,
            firstName,
            patronymic,
            address,
            phone
            )) {
        QMessageBox::critical(
            this,
            "Ошибка загрузки профиля",
            clientController.lastError()
            );
        return;
    }

    ui->lastNameLineEdit->setText(lastName);
    ui->firstNameLineEdit->setText(firstName);
    ui->patronymicLineEdit->setText(patronymic);
    ui->addressLineEdit->setText(address);
    ui->phoneLineEdit->setText(phone);
}

void ProfileWindow::onSaveButtonClicked()
{
    QString lastName = ui->lastNameLineEdit->text().trimmed();
    QString firstName = ui->firstNameLineEdit->text().trimmed();
    QString patronymic = ui->patronymicLineEdit->text().trimmed();
    QString address = ui->addressLineEdit->text().trimmed();
    QString phone = ui->phoneLineEdit->text().trimmed();

    ClientController clientController;

    if (!clientController.updateClientProfile(
            m_clientId,
            lastName,
            firstName,
            patronymic,
            address,
            phone
            )) {
        QMessageBox::warning(
            this,
            "Ошибка сохранения",
            clientController.lastError()
            );
        return;
    }

    QMessageBox::information(
        this,
        "Профиль обновлен",
        "Данные профиля успешно сохранены."
        );

    accept();
}

void ProfileWindow::onCloseButtonClicked()
{
    reject();
}