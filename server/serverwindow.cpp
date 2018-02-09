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

    qRegisterMetaTypeStreamOperators<User>("User");
    qMetaTypeId<User>();



    serverState = new QLabel;
    quitButton = new QPushButton(tr("Quitter"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    resize(350, 100);

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
 *    En effet, tant que la taille des informations de la scoket est inférieure à la taille du message qu'on attend,
 *    ça signifie qu'on a pas encore reçu le message en entier
 * 6) Une fois qu'on a le message en entier, on le met dans une QString (les strings de Qt) et on l'envoie à tous les clients
 * 7) on remet évidemment la taille du message à 0 pour les prochains messages
 *
*/
void ServerWindow::dataReceived()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;

    User* user;
    getUserBySocket(socket, user);

    QDataStream in(socket);

    cout<< "plop 1, message size = " << user->getSize() << endl;


    if (user->getSize() == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
             return;

        quint16 value;
        in >> value;
        user->setSize(value);
        cout<< "final  size = " << user->getSize() << endl;
    }

    if (socket->bytesAvailable() < user->getSize())
        return;
    cout<< "plop 2, size ? " << socket->bytesAvailable() << endl;


    QVariant serialized;
    //QVariant serialized = QVariant::fromValue(test);

    in >> serialized;

    User test =  serialized.value<User>();

    cout<< "plop 3 "  << endl;

    //test = serialized.value<User>();

    cout << "message received : " << test.getMessage().toStdString() << endl;
    QString message = test.getMessage();


    sendAll(message);
    saveInHistory(message);

    user->setSize(0);
}


void ServerWindow::newConnection()
{
       sendAll(tr("<em>Un nouveau client vient de se connecter</em>"));

       QTcpSocket *newSocket = server->nextPendingConnection();
       User* user = new User(newSocket);

       cout<< "new connection" << endl;

       clients << user;

       showHistory(user);

       connect(user->getSocket(), SIGNAL(readyRead()), this, SLOT(dataReceived()));
       connect(user->getSocket(), SIGNAL(disconnected()), this, SLOT(deconnexion()));
}

/*
 *  ici on prévient tout le monde de la déconnexion,
 *  puis on retire le client de la liste
 */
void ServerWindow::deconnexion()
{
    sendAll(tr("<em>Un client vient de se déconnecter</em>"));

    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;
    User* user;
    getUserBySocket(socket,user);

    clients.removeOne(user);

    socket->deleteLater();
}


/*
 * On ouvre le fichier History en mode append et on y insère le message
 */
void ServerWindow::saveInHistory(const QString &message)
{
    QFile file( "history.txt" );
    if ( file.open(QIODevice::WriteOnly| QIODevice::Append) )
    {
        QTextStream stream( &file );
        stream << message << endl;
    }
}


/*
 * On crée un paquet
 * Puis, on écrit le paquet dans toutes les socket (les clients)
 */
void ServerWindow::sendAll(const QString &message)
{
    QByteArray paquet = createPaquet(message);

    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->getSocket()->write(paquet);
    }
}

/*
 * On montre l'historique de la conversation à l'utilisateur
 */
void ServerWindow::showHistory(User *user)
{
    QFile file("history.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line = in.readLine();
    QString history("");
    history.append("<i><span style=color:#f26247;>Chat history :<br />");
    while (!line.isNull())
    {
        cout << "READING FILE, line = " << line.toStdString() << endl;
        history.append(line + "<br/>");
        line = in.readLine();
    }
    history.append("</span></i>");
    QByteArray paquet = createPaquet(history);
    user->getSocket()->write(paquet);
}




/*
 *
 * Ici, nous commençons par créer un Paquet, puis un Stream d'écriture sur celui-ci ('out')
 * NOus écrirons la taille du paquet plus tard, on commence donc par mettre une valeur par défault (0) de la taille
 * d'un quint16 afin de réserver cet espace mémoire du paquet
 * Ensuite, nous écrivons le message
 * Puis nous revenons au début du paquet et écrivons la taille de celui-ci
 */
QByteArray ServerWindow::createPaquet(const QString &message)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    return paquet;
}

void ServerWindow::getUserBySocket(QTcpSocket* socket,User*  user)
{
    for(int i=0; i< clients.length(); i++)
    {
        if(clients[i]->getSocket() == socket){
            user = clients[i];
            return;
        }

    }
        user = new User(socket);
}
