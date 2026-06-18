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

private:
    void loadClients();
    void loadOrders();

private:
    Ui::AdminWindow *ui;
    QSqlQueryModel *m_clientsModel;
    QSqlQueryModel *m_ordersModel;
};

#endif // ADMINWINDOW_H