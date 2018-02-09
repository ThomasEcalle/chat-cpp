#include <iostream>
#include "user.h"
using namespace std;

QDataStream & operator << (QDataStream & out, const User * user)
{
    cout << "client, out << message = " << user->getMessage().toStdString() << endl;

    out << user->getPseudo() << user->getMessage();

    return out;
}

QDataStream & operator >> (QDataStream & in, User * user)
{
    QString pseud;
    QString mess;
    in >> pseud;
    in >> mess;
    user->setPseudo(pseud);
    user->setMessage(mess);
    return in;
}

void User::initUserSystem()
{
    //qRegisterMetaTypeStreamOperators<User*>("User");
    //qMetaTypeId<User*>();
}


User::User() : pseudo("Anonyme")
{

}

User::User(QString pseudo, QString message, quint16 size)
{
    this->pseudo = pseudo;
    this->message = message;
    this->messageSize = size;
}

User::User(const User &other)
{
    this->pseudo = other.pseudo;
}

User::~User()
{

}

QString User::getPseudo() const
{
    return pseudo;
}

void User::setPseudo(QString pseudo)
{
    this->pseudo = pseudo;
}

QString User::getMessage() const
{
    return this->message;
}

void User::setMessage(QString message)
{
    this->message = message;
    cout << "thomaas set message : " << this->getMessage().toStdString() << endl;
}

quint16 User::getSize() const
{
    return messageSize;
}

void User::setSize(quint16 size)
{
    this->messageSize = size;
}
