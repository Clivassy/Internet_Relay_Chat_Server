
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

//-- Personnal Libraries 
#include "colors.hpp"

//-- Defines
#define PORT 6697



int main()
{
    //-- Server Process 
    //-- Vector to handle multiple connection to the server  

    int serverFd;
    int clientFd;
    int bufferSize = 1024;
    char buffer[bufferSize];

    //-- Socket()
    //-- Creation du socket 
    //  --> store the file descriptor returned by the socket : create a communication socket
    //-- AF_INET : indicate use of IPV4 Protocole
    //-- SOCK_STREAM : indicate use of TCP Protocole
    //-- 0 : indicates default protocole should be used.

    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0)
    {
        std::cout << BOLD_RED << "Error while creating socket" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << BOLD_YELLOW  << "Socket server created successfully" << RESET << std::endl;

    //-- Fill pollfd array 
    std::vector<pollfd> pollfdArray;
    pollfd server_pollfd = { serverFd, POLLIN, 0};
    pollfdArray.push_back(server_pollfd);

    //-- BIND
    //-- Enable to specify IP adress and Port Number to which the socket 
    //-- must be linked to communicate.
    struct sockaddr_in  serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // put automatic IP Adress
    // std::cout << "IP adress du server a transmettre au client: " << htonl(INADDR_ANY) << std:: endl;

    //-- Bind()
    if ( bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cout << BOLD_RED << "Error while bindind socket to Port 8080" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << BOLD_YELLOW  << "Socket bind successfully to port 8080" << RESET << std::endl;
    
    //-- Listen() --> wait for the client to approach the server to make a connection.
    // int listen(int sockfd, int backlog);
    // -- backlog == maximum 
    // SOMAXCONN defines the maximum number you're allowed to pass to listen().
    if (listen(serverFd, SOMAXCONN) < 0)
    {
        std::cout << BOLD_RED << "Error while listening" << RESET << std::endl;
        exit(EXIT_FAILURE);    
    }
    std::cout << BOLD_YELLOW  << "Server is listening..." << RESET << std::endl;

    //-- Accept() --> 
    struct sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);
    if ((clientFd = accept(serverFd,(struct sockaddr *)&clientAddr, &clientSize)) < 0)
    {
        std::cout << BOLD_RED << "Error while accepting connection" << RESET << std::endl;
        exit(EXIT_FAILURE);     
    }
    std::cout << BOLD_GREEN  << "Server catch connection" << RESET << std::endl;

    //-- read // wwrite : communication between processes
    //-- CLOSE 

    return (0);
}


