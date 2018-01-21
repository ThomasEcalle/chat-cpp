#ifndef HEADER_SERVERWINDOW
#define HEADER_SERVERWINDOW

#include <QtWidgets>
#include <QtNetwork>


class ServerWindow : public QWidget
{
    Q_OBJECT

    public:
        ServerWindow();
        void sendAll(const QString &message);

    private slots:
        void newConnection();
        void dataReceived();
        void deconnexion();

    private:
        QLabel *serverState;
        QPushButton *quitButton;

        QTcpServer *server;
        QList<QTcpSocket *> clients;
        quint16 messageSize;
};

#endif
