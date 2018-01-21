#include "serverwindow.h"


/*
 * Le constructeur va nous permettre de lancer le serveur
 * Il s'agit dans un premier temps de créer la fenêtre composée uniquement d'un champs texte et d'un bouton
 * Ensuite, nous tentons une connexion au serveur sur toutes les IP (locale, réseau interne et internet)
 * Sur le port 508885 (choisi au hasard..)
 * Puis nous indiquons via le champs text l'erreur s'il y en a ou alors la bonne connexion
 *
*/
ServerWindow::ServerWindow()
{
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



/*
 * Ici nous traitons les PAQUETS que nous recevons d'un client
 * Ces paquets sont certains d'arriver dans l'ordre (protocole TCP) mais on ne les reçois pas tous d'un coup
 * Un paquet possède une première partie indiquant la taille du message qui suit (sous fome de 'quint16', un petit INT)
 * L'idée ici est de :
 * 1) Récupérer la socket en castant le 'sender', si le résultat est à 0, c'est qu'on ne reconnait pas l'expediteur : on quitte la méthode
 * 2) on crée un Strem sur la coket pour pouvoir lire les données
 * 3) Tant qu'on a pas reçu la taille du message (tant que c'est plus petit qu'un 'quint16'), alors on ne fait rien
 * 4) UNe fois la taille reçue, on la renseigne dans notre variable
 * 5) Maintenant qu'on sait la taille du message, on va faire la même technique pour celui-ci
 * En effet, tant que la taille des informations de la scoket est inférieure à la taille du message qu'on attend,
 * ça signifie qu'on a pas encore reçu le message en entier
 * 6) Une fois qu'on a le message en entier, on le met dans une QString (les strings de Qt) et on l'envoie à tous les clients
 * 7) on remet évidemment la taille du message à 0 pour les prochains messages
 *
 *
*/
void ServerWindow::dataReceived()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;

    QDataStream in(socket);

    if (messageSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
             return;

        in >> messageSize;
    }

    if (socket->bytesAvailable() < messageSize)
        return;


    QString message;
    in >> message;


    sendAll(message);

    messageSize = 0;
}


void ServerWindow::newConnection()
{
       sendAll(tr("<em>Un nouveau client vient de se connecter</em>"));

       QTcpSocket *newClient = server->nextPendingConnection();
       clients << newClient;

       connect(newClient, SIGNAL(readyRead()), this, SLOT(dataReceived()));
       connect(newClient, SIGNAL(disconnected()), this, SLOT(deconnexion()));
}

void ServerWindow::deconnexion()
{

}

void ServerWindow::sendAll(const QString &message)
{

}
