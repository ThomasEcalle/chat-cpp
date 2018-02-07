#ifndef USER_H
#define USER_H

#include <QtWidgets>
#include <QtNetwork>

class User
{
public:
    User(QTcpSocket *socket, QString pseudo);
    QTcpSocket* getSocket();
    QString getPseudo();

private:
    QTcpSocket *socket;
    QString pseudo;
};

#endif // USER_H
