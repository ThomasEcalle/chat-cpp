#include "user.h"

QDataStream & operator << (QDataStream & out, const User & user)
{
    out << user.pseudo;
    out << user.message;
    out << user.messageSize;

    return out;
}

QDataStream & operator >> (QDataStream & in, User & user)
{
    QString pseud;
    QString mess;
    quint16 size;
    in >> pseud;
    in >> mess;
    in >> size;
    user = User(pseud, mess, size);
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