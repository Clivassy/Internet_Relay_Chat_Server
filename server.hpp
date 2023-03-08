#ifndef SERVER
# define SERVER

//-- Official libraries
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <poll.h>
#include <vector>
#include <map>

// Class Client 
class Client {
    public:
    //private:
        int socketFd;
    // stocker les differents clients : need to use poll
};

class User{
    // map here pour stocker les id des differents users 
};

// Class Server
class Server {
    public:
        Server(){}
        ~Server(){}

    //private:
        int     bufferSize = 1024;
        int     socketFd;
        int     port;
        char    buffer[1024];
};

// Class Channel 
class Channel{
    // assez flou pour l'in stant encore 
};

// Class Message
class Message{
    //-- with sender, receiver, command calles ect

};
#endif 