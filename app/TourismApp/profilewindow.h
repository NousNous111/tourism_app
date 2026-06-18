#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QDialog>

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileWindow(int clientId, QWidget *parent = nullptr);
    ~ProfileWindow();

private slots:
    void onSaveButtonClicked();
    void onCloseButtonClicked();

private:
    void loadProfile();

private:
    Ui::ProfileWindow *ui;
    int m_clientId;
};

#endif // PROFILEWINDOW_H