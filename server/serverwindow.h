#ifndef HEADER_SERVERWINDOW
#define HEADER_SERVERWINDOW

#include <QtWidgets>
#include <QtNetwork>
#include <iostream>
#include "user.h"
using namespace std;


Q_DECLARE_METATYPE(User*)

class ServerWindow : public QWidget
{
    Q_OBJECT

    public:
        ServerWindow();
        void sendAll(const QString &message);
        void showHistory(User *user);
        void saveInHistory(const QString &message);

    private slots:
        void newConnection();
        void dataReceived();
        void deconnexion();


    private:
        QByteArray createPaquet(const QString &message);
        void getUserBySocket(QTcpSocket* socket,User*  user);
        QLabel *serverState;
        QPushButton *quitButton;

        QTcpServer *server;
        QList<User *> clients;
};

#endif
