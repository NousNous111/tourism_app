#ifndef MYORDERSWINDOW_H
#define MYORDERSWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class MyOrdersWindow;
}

class MyOrdersWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MyOrdersWindow(int clientId, QWidget *parent = nullptr);
    ~MyOrdersWindow();

private:
    void loadOrders();

private:
    Ui::MyOrdersWindow *ui;
    QSqlQueryModel *m_ordersModel;
    int m_clientId;
};

#endif // MYORDERSWINDOW_H