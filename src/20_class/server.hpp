#ifndef SERVER_HPP
# define SERVER_HPP

//# include "../main.hpp"
# include <iostream>
# include <string>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <netdb.h>
# include <poll.h>
# include <vector>
# include <map>
# include "../10_tools/colors.hpp"
# include "client.hpp"


# define BUFFER_SIZE 1024
# define LISTENING_TIMEOUT 10000 // delai d'ecoute dans poll() en ms

class Client;

class Server
{
	protected:
		int			port;
		std::string	password;

		//const int			bufferSize;
		//char				buffer[BUFFER_SIZE];
		int					socketFd;
		int					serverFd;
		struct sockaddr_in	serverAddr;
		std::vector<pollfd> fdListened;
		std::vector<Client> clientList;		

		//TBD 
		// ajouter tableau de client
		// tableau de pollfd


	public:
		Server();
		void set_port(int port);
		void set_password(std::string password);
		void init();
		void run();
		void manage_poll_event();
		void addNewClient();
		void listen_client(Client &client);
		Client& getClient(int fd);

		void terminate();
};

#endif 
