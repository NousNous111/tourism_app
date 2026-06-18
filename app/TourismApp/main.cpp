#include "mainwindow.h"
#include "loginwindow.h"
#include "database/databasemanager.h"

#include <QApplication>
#include <QMessageBox>
#include <QDialog>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    if (!DatabaseManager::instance().connectToDatabase()) {
        QMessageBox::critical(
            nullptr,
            "Ошибка подключения к базе данных",
            DatabaseManager::instance().lastError()
            );
        return 1;
    }

    while (true) {
        LoginWindow loginWindow;

        if (loginWindow.exec() != QDialog::Accepted) {
            break;
        }

        MainWindow mainWindow(
            loginWindow.userId(),
            loginWindow.clientId(),
            loginWindow.userRole()
            );

        bool logoutRequested = false;

        QObject::connect(
            &mainWindow,
            &MainWindow::logoutRequested,
            [&logoutRequested]() {
                logoutRequested = true;
            }
            );

        mainWindow.show();

        application.exec();

        if (!logoutRequested) {
            break;
        }
    }

    DatabaseManager::instance().closeConnection();

    return 0;
}