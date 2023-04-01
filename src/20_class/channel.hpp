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
		bool	isclientConnected(std::string name);
		bool	isClientBanned(std::string name);
		bool	isClientOperatorChannel(std::string name);
		void	sendMessageToClients( std::string msg, std::string sender);
		void	addClient(Client& client);
		

	public: // TBD remettre en protected avec friend ou getteur
		Server&						server;
		std::string					name;
		std::string					topic; // TBD voir on implemente le topic et de cette facon (c'est actuellement utilise pour l'arrivee dans un channel)
		std::set<std::string>		clientConnected; // userName is used to identify a client
		std::set<std::string>		clientBanned; // userName is used to identify a client
		std::set<std::string>		clientOperators; 
		bool						isInviteOnly;
};

#endif
