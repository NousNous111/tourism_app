#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QString>

#include "models/user.h"

class AuthController
{
public:
    AuthController();

    bool login(const QString &login, const QString &password);

    User currentUser() const;
    int currentClientId() const;
    QString lastError() const;

private:
    QString hashPassword(const QString &password) const;

private:
    User m_currentUser;
    int m_currentClientId;
    QString m_lastError;
};

#endif // AUTHCONTROLLER_H