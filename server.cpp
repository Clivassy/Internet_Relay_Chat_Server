
#include <iostream>
#include <string>
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

//-- Personnal Libraries 
#include "colors.hpp"

#define BUFFER_SIZE 1024

// TBD passer class dans fichiers .cpp et .hpp a part
class Server
{
	protected:
		int			port;
		std::string	password;

		const int			bufferSize = BUFFER_SIZE;
		char				buffer[BUFFER_SIZE];
		int					socketFd;
		int					serverFd;
		struct sockaddr_in	serverAddr;
		//TBD 
		// ajouter tableau de client
		// tableau de pollfd


	public:
		void set_port(int port){this->port = port;}
		void set_password(std::string password){this->password = password;}
		void init()
		{
			memset(this->buffer, 0, bufferSize); // TBD voir si il faut pas remplir de bufferS0ze * sizeof(int)
			this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
			if (this->serverFd < 0)
			{
				std::cout << BOLD_RED << "Error while creating socket" << RESET << std::endl;
				exit(EXIT_FAILURE);
			}
			std::cout << BOLD_GREEN  << "Socket server created successfully" << RESET << std::endl;
			this->serverAddr.sin_family = AF_INET;
			this->serverAddr.sin_port = htons(this->port);
			this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			if (bind(this->serverFd, (struct sockaddr*)&(this->serverAddr), sizeof(this->serverAddr)) < 0)
			{
				std::cout << BOLD_RED << "Error while bindind socket to Port " << this->port << RESET << std::endl;
				exit(EXIT_FAILURE);
			}
			std::cout << BOLD_GREEN  << "Socket bind successfully to port " << this->port << RESET << std::endl;

			//TBD fonction pour print les attributs du server
		}
		void run()
		{
			int i = 0; // limite pour eviter de kill process en debug, TBD mettre loop infinie
			// TBD redefinir ctrl+C pour close les fd (evite de bloquer les ports en debug)
			while(i < 8)
			{
				std::cout << BOLD_BLUE<< "loop step " + std::to_string(i) + "\n" << RESET << std::endl;

				// ecoute nouvelle connection (accept) -> ajout client si nouvelle connexion entrante

				// ecoute des clients (poll) -> lancement commande si message recu 
					// parsing command
					// execution command



				// temporisation pour debug (TBD a enlever a la fin)
				sleep(1); // sleep 1s
				i++;
			}
		}
		void terminate()
		{
			close (this->serverFd);
			//close (this->clientFd); // TBD close les fd clients
		}


};

void parsing_arguments(int ac, char **av, Server& server)
{
	// av[1] == port 
	// av[2] == password
	if (ac != 3)
	{
		std::cout << BOLD_RED << "Wrong input arguments" << std::endl;
		std::cout << "Ex: ./ircserv [port] [password]" << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	// TBD a ajouter
	//  - validité port
	//  - validité passsword

	server.set_port(atoi(av[1]));
	server.set_password(av[2]);
}


int main(int ac, char **av)
{
	Server server;
	parsing_arguments(ac, av, server);
	server.init();
	server.run();
	server.terminate();



}
