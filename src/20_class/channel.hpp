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

class Server;

class Channel
{
	public:
		Channel( Server& serv );
		~Channel( void );
		void	sendMessageToClients( std::string msg);
		void	addMsgToHistory(std::string msg);
		void    addClientToChannel( void );
	
	protected:
		Server&						server;
		std::set<int>				clientConnected;
};

#endif
