#ifndef SERVER_HPP
# define SERVER_HPP

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
# define LISTENING_TIMEOUT 10000// delai d'ecoute dans poll() en ms
# define PING_FREQUENCY 120000// frequence d'envoie de ping au clients en ms (120ms = temps habituel)
# define WAIT_TIME_BEFORE_KILL 19000// temps d'attente apres un ping avant de supprimer client en ms

class Client;
class Channel;

class Server
{
	friend class Channel;
	friend class Client;

	protected:
		int								port;
		std::string						password;
		int								socketFd;
		int								serverFd;
		struct sockaddr_in				serverAddr;
		std::vector<pollfd> 			fdListened;
		std::vector<Client> 			clientList;
		std::map<std::string, Channel>	channelList;

	public:
		// Server
		Server();
		void										set_port(int port);
		void										set_password(std::string password);
		std::string									get_password(void);
		void										init();
		void										run(int i);
		void										terminate();

		// client
		void										manage_poll_event();
		void										addNewClient();
		void										listen_client(Client &client);
		std::vector<pollfd>::iterator				getPollfd(int fd);
		std::vector<Client>::iterator				getClient(int fd);
		std::vector<Client>::iterator				getClient(std::string user);
		std::vector<pollfd>::iterator				getClientByFd(std::string user);
		void										pingAllClients();
		void										checkInactiveClients();;
		size_t										nbOfClientsNotOnline();
		void										removeFirstClientNotOnline();
		void										removeNotOnlineClient(void);
		void										removeFirstChannelEmpty();
		size_t										nbOfEmptyChannels();
		void										removeEmptyChannels();
		void										removeClient(std::string name);
		void										killclient(std::string name, std::string reason);

		// channel
		bool										isChannelExisting(std::string name);
		bool										isClientExisting(std::string name);
		void										addChannel(std::string name);
		std::map<std::string, Channel>::iterator	getChannel(std::string name);

		// debug
		void		printState();
};
#endif 
