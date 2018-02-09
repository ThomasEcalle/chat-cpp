#ifndef USER_H
#define USER_H

#include <QtWidgets>
#include <QtNetwork>


class User
{

public:
    User();
    User(QTcpSocket* socket);
    User(QString pseudo, QString message, quint16 size);
    QTcpSocket* getSocket() const;
    QString getPseudo() const;
    void setPseudo(QString pseudo);
    QString getMessage() const;
    void setMessage(QString message);
    quint16 getSize() const;
    void setSize(quint16 size);

    User(const User &other);
    ~User();

private:
    QString pseudo;
    QString message;
    QTcpSocket* socket;
    quint16 messageSize;

    friend QDataStream & operator << (QDataStream &, const User *);
    friend QDataStream & operator >> (QDataStream &, User *);

};
//Q_DECLARE_METATYPE(User*)
QDataStream & operator << (QDataStream & out, const User * user);
QDataStream & operator >> (QDataStream & in, User * user);



#endif // USER_H

