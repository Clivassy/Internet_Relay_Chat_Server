#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
    //-- STEP 1) Establish a connection between a client and serve using TCP protocole.
    //-- 
    int client, server; // file descriptors for the client and servers sockets.
    int portNumber = 1500;
    bool isExit = false; // indicate whether the server should continue running
    int buffsize = 1024; // size of the buffer
    char buffer[buffsize]; // buffer used to send and receive data.

    struct sockaddr_in_server_addr; // create a new sockadd_in structure for the server's adress
    // used to bind the server socket to a specific IP adress and port.

    socklen_t size; // used to store the size of the sockaddr_in

    //-- Init socket 
    client = socket(AF_INET, SOCK_STREAM, 0); 
    //-- create a new socket
    //-- AF_INET --> specifies we are using the IPv4 Protocol.
    //-- SOCK_STREAM --> specifies we are using TCP Protocol.
    //-- `0` --> specifies the default protocol for the socket.

    //-- Protection
    //-- Checks if sockeet returned a valid file descriptor.
    if (client < 0)
    {
        cout << "Error establishing connection." << endl;
        exit(1);
    }
    return (0);
}