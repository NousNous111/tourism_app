#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    int userId() const;
    int clientId() const;
    QString userRole() const;

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();

private:
    QString hashPassword(const QString &password) const;

private:
    Ui::LoginWindow *ui;

    int m_userId;
    int m_clientId;
    QString m_userRole;
};

#endif // LOGINWINDOW_H