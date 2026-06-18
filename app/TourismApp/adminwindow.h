#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void onRefreshButtonClicked();
    void onAddPackageButtonClicked();
    void onDeletePackageButtonClicked();

private:
    void loadClients();
    void loadOrders();
    void loadPackages();

private:
    Ui::AdminWindow *ui;
    QSqlQueryModel *m_clientsModel;
    QSqlQueryModel *m_ordersModel;
    QSqlQueryModel *m_packagesModel;
};

#endif // ADMINWINDOW_H