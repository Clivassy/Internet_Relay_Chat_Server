#ifndef CHANNEL_HPP
# define CHANNEL_HPP

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
# include <set>
# include "server.hpp"
# include "./client/client.hpp"

class Server;
class Client;

// Regles selon norme pour nommage channel :
// Commence par '&', '#', '+' ou '!'
// longueur max 50 characteres
// ne contient pas d'espace ' ', control G (^G / ASCII 7) ou virgule (',')
// non sensible a la casse
class Channel
{
	public:
		Channel( Server& serv, std::string channel_name );
		~Channel( void );
		void	sendMessageToClients( std::string msg, std::string sender);
		void	addMsgToHistory(std::string msg);
		void	addClient(Client& client);

	public: // TBD remettre en protected avec friend ou getteur
		Server&						server;
		std::string					name;
		std::set<std::string>				clientConnected; // userName is used to identify a client
};

#endif
