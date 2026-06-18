#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <QString>

class ClientController
{
public:
    ClientController();

    bool registerClient(
        const QString &login,
        const QString &password,
        const QString &lastName,
        const QString &firstName,
        const QString &patronymic,
        const QString &address,
        const QString &phone
        );

    QString lastError() const;

private:
    bool isPasswordValid(const QString &password) const;
    QString hashPassword(const QString &password) const;
    QString sqlValue(const QString &value) const;

private:
    QString m_lastError;
};

#endif // CLIENTCONTROLLER_H