#include "clientwindow.h"

/*
 * Tout d'abord, on met en place l'UI
 * Ensuite, on initialise notre pointeur représentant le Serveur
 * Puis on Bing les différends signaux du server avec nos méthodes :
 * Connection, réception de paquet, déconnextion et erreure sur la socket
 */
ClientWindow::ClientWindow()
{
    setupUi(this);

    qRegisterMetaTypeStreamOperators<User>("User");
    qMetaTypeId<User>();

    pseudo->setEnabled(false);
    message->setEnabled(false);

    socket = new QTcpSocket(this);

    user = User();
    user.setSize(0);


    QObject::connect(socket, &QTcpSocket::readyRead, this, &ClientWindow::dataReceived);
    QObject::connect(socket, &QTcpSocket::connected, this, &ClientWindow::connect);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &ClientWindow::deconnexion);
    QObject::connect(socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &ClientWindow::socketError);

}

/*
 * Tentative de connexion au serveur
 * On affiche un message de connexion
 * On annule toute pecédente tentative de connexion, puis on
 * se connecte à l'IP et au port choisis
 */
void ClientWindow::on_connectButton_clicked()
{
    // On annonce sur la fenêtre qu'on est en train de se connecter
    messagesList->append(tr("<em>Tentative de connexion en cours...</em>"));
    connectButton->setEnabled(false);

    socket->abort();
    socket->connectToHost(serverIP->text(), serverPort->value());
}


/*
 *Ici, nous commençons par créer un Paquet, puis un Stream d'écriture sur celui-ci ('out')
 * NOus écrirons la taille du paquet plus tard, on commence donc par mettre une valeur par défault (0) de la taille
 * d'un quint16 afin de réserver cet espace mémoire du paquet
 * Ensuite, nous écrivons le message (en y ajoutant le epseudo choisi)
 * Puis nous revenons au début du paquet et écrivons la taille de celui-ci
 *
 * Enfin, on écrit le paquet sur le socket du serveur, puis on réinitialise le champs d'étition
 *
 */
void ClientWindow::on_send_clicked()
{

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);



    // On prépare le paquet à envoyer
    user.setMessage(message->text());
    cout<< "preparing message ton send : " << user.getMessage().toStdString() << endl;
    //QString messageToSend = tr("<strong>") + pseudo->text() +tr("</strong> : ") + message->text();

    QVariant serializedUser;
    serializedUser.setValue(user);

    cout << "serialized user correctly" << endl;

    out << (quint16) 0;
    out << serializedUser;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    cout << "before writting" << endl;

    socket->write(paquet); // On envoie le paquet

    cout << "we send packet" << endl;

    message->clear(); // On vide la zone d'écriture du message
    message->setFocus(); // Et on remet le curseur à l'intérieur
}


/*
 * Simple Tricks pour envoyer le message lorsqu'on appuie sur "entrée"
 */
void ClientWindow::on_message_returnPressed()
{
    on_send_clicked();
}


/*
 * Ici, on crée un Stream pour lire la socket du serveur
 * Tant qu'on a pas récupérer assez de paquets pour avoir la taille du message,
 * on ne fait rien
 * Une fois qu'on a la taille, on réitère le même principe tant qu'on a pas
 * le message en entier
 * Enfin, on récupère le message, on l'affiche à l'écran et on réinitialise
 * la variable de Taille de message à 0
 */
void ClientWindow::dataReceived()
{
    QDataStream in(socket);

    if (messageSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
             return;

        in >> messageSize;
    }

    if (socket->bytesAvailable() < messageSize)
        return;


    QVariant received;
    in >> received;

    User user = received.value<User>();

    messagesList->append(user.getMessage());

    messageSize = 0;

}


/*
 * Slot appelé lorsque la socket serveur signale qu'on est bien connecté
 */
void ClientWindow::connect()
{
    messagesList->append(tr("<em>Connexion réussie !</em>"));
    connectButton->setEnabled(false);
    pseudo->setEnabled(true);
    message->setEnabled(true);
}

/*
 * Slot appelé lorsque la socket serveur signale la déconnexion
 */
void ClientWindow::deconnexion()
{
    messagesList->append(tr("<em>Déconnecté du serveur</em>"));
}



/*
 * Slot appelé lorsque la socket serveur renvoi une erreur
 */
void ClientWindow::socketError(QAbstractSocket::SocketError error)
{
    switch(error)
    {
        case QAbstractSocket::HostNotFoundError:
            messagesList->append(tr("<em>ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            messagesList->append(tr("<em>ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            messagesList->append(tr("<em>ERREUR : le serveur a coupé la connexion.</em>"));
            break;
        default:
            messagesList->append(tr("<em>ERREUR : ") + socket->errorString() + tr("</em>"));
    }

    connectButton->setEnabled(true);
}













