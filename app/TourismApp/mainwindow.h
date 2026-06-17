#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int userId, int clientId, const QString &userRole, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRefreshButtonClicked();
    void onInterestButtonClicked();
    void onOrderButtonClicked();

private:
    void loadTravelPackages();
    void configureInterfaceByRole();

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *m_packagesModel;

    int m_userId;
    int m_clientId;
    QString m_userRole;
};

#endif // MAINWINDOW_H