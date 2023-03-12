
#include <stdlib.h>
#include <string>
//-- Personnal Libraries 
#include "colors.hpp"
#include "server.hpp"


int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << BOLD_RED << "Wrong input arguments" << std::endl;
        std::cout << "Ex: ./ircserv [port] [password]" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    //--- MINI PARSING ARG ---------------//
    // checker si av[1] == port 
    // checker si av[2] == password
    //-------------------------------------//
    
    Server _server;
    int serverFd;
    int clientFd;
    int bufferSize = 1024;
    char buffer[bufferSize];

    _server.port = atoi(av[1]);
    memset(buffer, 0, bufferSize);
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

    //-- BIND
    //-- Enable to specify IP adress and Port Number to which the socket 
    //-- must be linked to communicate.

    //--> avant le bind, fonction d'initialisation pr attributs server
    struct sockaddr_in  serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_server.port);
    std::cout << "Port: " << serverAddr.sin_port << std::endl;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // put automatic IP Adress
    // std::cout << "IP adress du server a transmettre au client: " << htonl(INADDR_ANY) << std:: endl;

    //-- Bind()
    if ( bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cout << BOLD_RED << "Error while bindind socket to Port " << _server.port << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << BOLD_YELLOW  << "Socket bind successfully to port " << _server.port << RESET << std::endl;
    
    //-- Listen() --> wait for the client to approach the server to make a connection.
    // int listen(int sockfd, int backlog);
    // -- backlog == maximum 
    // SOMAXCONN defines the maximum number you're allowed to pass to listen().
    if (listen(serverFd, 5) < 0)
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

    //-- Receive message from client
    //-- Parsing des command here ? 
    //-- Dans quoi stocker les commandes + clear le buffer at each time
    size_t responseSize = 1;
    std::string msg("");
     if (responseSize > 0){
        std::cout << "debut loop" << responseSize << std::endl;
        responseSize = recv(clientFd, buffer, bufferSize -1, 0);
        if ( responseSize > 0)
        msg += buffer;
        std::cout << "Buffer: " << buffer << std::endl;
        // std::cout << "Message from client: " << msg << "size = " << responseSize << std::endl;
        std::cout << "fin loop" << responseSize << std::endl;
    }
	std::string answer = "001 ybellot :Welcome to the Internet Relay Network ybellot!ybellot@locahost";
    std::cout << "answer= " << answer << std::endl;

	//001    RPL_WELCOME
    //   "Welcome to the Internet Relay Network
    //    <nick>!<user>@<host>"
    //std::cout << "Message from client: " << msg << "size = " << responseSize << std::endl;
    
    //size_t responseSize = recv(clientFd, buffer, bufferSize -1, 0);
    //std::cout << "Message from client: " << buffer << "size = " << responseSize << std::endl;

    //-- Gerer la creation de channels
    //--> begin with & or #  character => its name can't contain more than 200 characters

    //-- send message to the connection 
    //std::cout << "here -> " << std::endl;
    //std::string response = "Message from server: bien reçu\n";
    //send(clientFd, response.c_str(), response.size(), 0);
    send(clientFd, answer.c_str(), answer.size(), 0);



    std::cout << BOLD_YELLOW << "loop inf" << RESET << std::endl;

	int i = 0;
	while(i < 7)
	{
		answer = "message from server " + std::to_string(i) + "\n";
		std::cout << BOLD_BLUE<< "message send= " << answer << RESET << std::endl;
   		send(clientFd, answer.c_str(), answer.size(), 0);
		responseSize = recv(clientFd, buffer, bufferSize -1, 0);
		std::cout << "buffer= " << buffer << std::endl;

		sleep(1);
		i++;
	}

	    //-- CLOSE 
    close (clientFd);
    close (serverFd);

    return (0);
}
//
//-- listen server avec nc : nc -l -p 8080
//-- 
