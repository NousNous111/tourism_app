#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User()
        : idUser(-1)
        , login("")
        , role("")
    {
    }

    User(int idUser, const QString &login, const QString &role)
        : idUser(idUser)
        , login(login)
        , role(role)
    {
    }

    int idUser;
    QString login;
    QString role;
};

#endif // USER_H