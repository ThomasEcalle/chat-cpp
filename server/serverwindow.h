#ifndef HEADER_SERVERWINDOW
#define HEADER_SERVERWINDOW

#include <QtWidgets>
#include <QtNetwork>
#include <iostream>
using namespace std;



class ServerWindow : public QWidget
{
    Q_OBJECT

    public:
        ServerWindow();
        void sendAll(const QString &message);
        void showHistory(QTcpSocket *user);
        void saveInHistory(const QString &message);

    private slots:
        void newConnection();
        void dataReceived();
        void deconnexion();


    private:
        QByteArray createPaquet(const QString &message);
        QLabel *serverState;
        QPushButton *quitButton;

        QTcpServer *server;
        QList<QTcpSocket *> clients;
        quint16 messageSize;
};

#endif
