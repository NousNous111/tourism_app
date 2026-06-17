#include "mainwindow.h"
#include "loginwindow.h"
#include "database/databasemanager.h"

#include <QApplication>
#include <QMessageBox>
#include <QDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!DatabaseManager::instance().connectToDatabase()) {
        QMessageBox::critical(
            nullptr,
            "Ошибка подключения",
            "Не удалось подключиться к базе данных:\n" +
                DatabaseManager::instance().lastError()
            );
        return 1;
    }

    LoginWindow loginWindow;

    if (loginWindow.exec() != QDialog::Accepted) {
        return 0;
    }

    MainWindow w(
        loginWindow.userId(),
        loginWindow.clientId(),
        loginWindow.userRole()
        );

    w.show();

    return QApplication::exec();
}