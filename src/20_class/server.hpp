#ifndef SERVER_HPP
# define SERVER_HPP

//# include "../main.hpp"
# include <algorithm>
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
# include "../10_tools/utils.hpp"
# include "client/client.hpp"
# include "channel.hpp"


# define BUFFER_SIZE 1024
# define LISTENING_TIMEOUT 30000// delai d'ecoute dans poll() en ms

class Client;
class Channel;

class Server
{
	public:
	//protected:
		int			port;
		std::string	password;

		int								socketFd;
		int								serverFd;
		struct sockaddr_in				serverAddr;
		std::vector<pollfd> 			fdListened;
		std::vector<Client> 			clientList;		
		std::map<std::string, Channel>	channelList;

	public:
		// Server
		Server();
		void		set_port(int port);
		void		set_password(std::string password);
		std::string	get_password(void);
		void		init();
		void		run();
		void		terminate();

		// client
		void							manage_poll_event();
		void							addNewClient();
		void							listen_client(Client &client);
		std::vector<Client>::iterator	getClient(int fd);
		std::vector<Client>::iterator	getClient(std::string user);


		// channel
		bool										isChannelExisting(std::string name);
		bool										isClientExisting(std::string name);
		void										addChannel(std::string name);
		std::map<std::string, Channel>::iterator	getChannel(std::string name);
		//Channel&	getChannel(std::string name);


		// debug
		void		printState();

};

#endif 
