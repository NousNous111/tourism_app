#ifndef INTERESTEDPACKAGESWINDOW_H
#define INTERESTEDPACKAGESWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class InterestedPackagesWindow;
}

class InterestedPackagesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InterestedPackagesWindow(int clientId, QWidget *parent = nullptr);
    ~InterestedPackagesWindow();

private:
    void loadInterestedPackages();

private:
    Ui::InterestedPackagesWindow *ui;
    QSqlQueryModel *m_interestedModel;
    int m_clientId;
};

#endif // INTERESTEDPACKAGESWINDOW_H