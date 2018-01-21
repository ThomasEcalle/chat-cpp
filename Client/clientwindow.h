#ifndef HEADER_CLIENTWINDOW
#define HEADER_CLIENTWINDOW

#include <QtWidgets>
#include <QtNetwork>
#include "ui_ClientWindow.h"

/*
 * Notre fenêtre côté client possède un fichier UI généré par Qt Designer car plus "complexe" que celle du serveur
 * On rerouve ici les différents slots de notre classe ainsi qu'une socket representant le serveur et une variable
 * de taille de message (même principe que côté serveur)
*/

class ClientWindow : public QWidget, private Ui::ClientWindow
{
    Q_OBJECT

    public:
        ClientWindow();

    private slots:
        void on_connect_clicked();
        void on_send_clicked();
        void on_message_returnPressed();
        void dataReceived();
        void connect();
        void deconnexion();
        void socketError(QAbstractSocket::SocketError error);

    private:
        QTcpSocket *serverSocket;
        quint16 messageSize;
};

#endif
