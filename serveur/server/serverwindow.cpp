#include "serverwindow.h"

ServerWindow::ServerWindow()
{
    // Création et disposition des widgets de la fenêtre
    serverState = new QLabel;
    quitButton = new QPushButton(tr("Quitter"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(serverState);
    layout->addWidget(quitButton);
    setLayout(layout);

    setWindowTitle(tr("Chat, groupe 2"));

    // Gestion du serveur
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, 50885)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50585
    {
        // Si le serveur n'a pas été démarré correctement
        serverState->setText(tr("Le serveur n'a pas pu être démarré. Raison :<br />") + server->errorString());
    }
    else
    {
        // Si le serveur a été démarré correctement
        serverState->setText(tr("Le serveur a été démarré sur le port <strong>") + QString::number(server->serverPort()) + tr("</strong>.<br />Des clients peuvent maintenant se connecter."));
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    }

    messageSize = 0;
}


void ServerWindow::dataReceived()
{

}


void ServerWindow::newConnection()
{

}

void ServerWindow::deconnexion()
{

}
