#include <iostream>
#include "user.h"
using namespace std;

QDataStream & operator << (QDataStream & out, const User * user)
{
    out << user->pseudo;
    out << user->message;
    out << user->messageSize;

    return out;
}

QDataStream & operator >> (QDataStream & in, User *user)
{
    cout << "in operator << from User" << endl;

    QString pseud;
    QString mess;
    quint16 size;
    in >> pseud;
    in >> mess;
    in >> size;

    cout << "in operator << from User message : " << mess.toStdString() << " pseudo :" << pseud.toStdString() << endl;

    user = new User(pseud, mess, size);
    return in;
}

User::User() : pseudo("Anonyme")
{

}

User::User(QTcpSocket* socket) : pseudo("")
{
    this->socket = socket;
}

User::User(QString pseudo, QString message, quint16 size)
{
    cout << "constructor User" <<endl;
    this->pseudo = pseudo;
    this->message = message;
    this->messageSize = size;

    cout << "constructor User, pseudo : " << this->pseudo.toStdString() <<endl;
    cout << "constructor User, message : " << this->message.toStdString() <<endl;
}

User::User(const User &other)
{
    this->pseudo = other.pseudo;
}

User::~User()
{

}

QTcpSocket* User::getSocket() const
{
    return socket;
}

QString User::getPseudo() const
{
    return pseudo;
}

void User::setPseudo(QString pseudo)
{
    cout << "in pseudo setter, value to set id : " << pseudo.toStdString() << endl;
    this->pseudo = pseudo;
    cout << "baobaaaaab" << endl;
}

QString User::getMessage() const
{
    return message;
}

void User::setMessage(QString message)
{
    this->message = message;
}

quint16 User::getSize() const
{
    return messageSize;
}

void User::setSize(quint16 size)
{
    this->messageSize = size;
}
