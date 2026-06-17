#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void onRegisterButtonClicked();
    void onCancelButtonClicked();

private:
    bool isPasswordValid(const QString &password) const;
    QString hashPassword(const QString &password) const;

private:
    Ui::RegisterWindow *ui;
};

#endif // REGISTERWINDOW_H