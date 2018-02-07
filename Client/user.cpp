#include "user.h"


User::User(QTcpSocket *userSocket, QString userPseudo)
{
    pseudo = userPseudo;
    socket = userSocket;
}
