#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

class Client
{
public:
    Client() = default;

    Client(
        int idClient,
        int idUser,
        const QString& lastName,
        const QString& firstName,
        const QString& patronymic,
        const QString& address,
        const QString& phone
    )
        : m_idClient(idClient),
          m_idUser(idUser),
          m_lastName(lastName),
          m_firstName(firstName),
          m_patronymic(patronymic),
          m_address(address),
          m_phone(phone)
    {
    }

    int idClient() const { return m_idClient; }
    int idUser() const { return m_idUser; }

    QString lastName() const { return m_lastName; }
    QString firstName() const { return m_firstName; }
    QString patronymic() const { return m_patronymic; }
    QString address() const { return m_address; }
    QString phone() const { return m_phone; }

    QString fullName() const
    {
        return m_lastName + " " + m_firstName + " " + m_patronymic;
    }

private:
    int m_idClient = -1;
    int m_idUser = -1;
    QString m_lastName;
    QString m_firstName;
    QString m_patronymic;
    QString m_address;
    QString m_phone;
};

#endif // CLIENT_H
