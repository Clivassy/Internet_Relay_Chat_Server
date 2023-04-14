#include "server.hpp"

Server::Server():
port(-1), serverFd(-1)
{}

std::vector<pollfd>::iterator	Server::getPollfd(int fd_searched)
{
	std::vector<pollfd>::iterator it;
	for (it = this->fdListened.begin(); it != this->fdListened.end(); it++)
		{
			if (it->fd == fd_searched)
				return (it);
		}
	return (it); // return end si fd non trouvé
}

std::vector<Client>::iterator Server::getClient(int fd)
{
	std::vector<Client>::iterator it;
	for (it = this->clientList.begin(); it != this->clientList.end(); it++)
		{
			if (it->socketFd == fd)
				return (it);
		}
	return (it); // return end si fd non trouvé
}

std::vector<Client>::iterator Server::getClient(std::string user)
{
	std::vector<Client>::iterator it;
	for (it = this->clientList.begin(); it != this->clientList.end(); it++)
		{
			if (it->userInfos.nickName == user)
				return (it);
		}
	return (it); // return end si fd non trouvé
}

std::vector<pollfd>::iterator Server::getClientByFd(std::string user)
{
	std::vector<Client>::iterator it;
	for (it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		if (it->userInfos.nickName == user)
			break ;
	}
	std::vector<pollfd>::iterator its;
	for (its = this->fdListened.begin(); its != this->fdListened.end(); its++)
	{
		if (its->fd == it->socketFd)
			return (its);
	}
	return (its); // return end si fd non trouvé
}

// ping les client connectés toutes les PING_FREQUENCY ms
void	Server::pingAllClients()
{
	for(std::vector<Client>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		if (it->status == CONNECTED)
		{
			if (difftime(time(0), it->lastPingSent) > PING_FREQUENCY / 1000)
			{
				it->ping();
				it->lastPingSent = time(0);
			}
		}
	}
}

void Server::checkInactiveClients()
{
	// TBD utiliser des [i] au lien des it pour ne pas invalider les it quand il y a des suppressions
	// ou passer revent a -1 et kill les clients a la fin de la boucle dans le run
	for(std::vector<Client>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		// WAIT_TIME_BEFORE_KILL = xxxxxxx
		// -----ping-pong---------ping-pong----------ping----time(0)
		// -------|---|-------------|---|-------------|------|--------
		// --------xxx---------------xxx---------------xxxxxxx <- kill
		// 1er check equivaut a regarder si un ping a ete envoyé (ping>pong) sinon ca veut dire qu'on a deja eu la reponse au dernier ping
		// 2n check pour voir si le pong met trop de temps a revenir apres le dernier ping
		if (difftime(it->lastPingSent, it->lastPongReceived) > 0 && difftime(time(0), it->lastPingSent) > WAIT_TIME_BEFORE_KILL / 1000)
		{
			it->online = false;
		}
	}
}

void Server::set_port(int port)
{
	this->port = port;
}

void Server::set_password(std::string password)
{
	this->password = password;
}

std::string Server::get_password(void)
{
	return (this->password);
}

void Server::init()
{
	this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverFd < 0)
	{
		std::cout << BOLD_RED << "Error while creating socket" << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << BOLD_GREEN << "Socket server created successfully" << RESET << std::endl;
	this->serverAddr.sin_family = AF_INET;
	this->serverAddr.sin_port = htons(this->port);
	this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->serverFd, (struct sockaddr *)&(this->serverAddr), sizeof(this->serverAddr)) < 0)
	{
		std::cout << BOLD_RED << "Error while bindind socket to Port " << this->port << std::endl;
		perror("bind");
		std::cout << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << BOLD_GREEN << "Socket bind successfully to port " << this->port << RESET << std::endl;
	// ecoute sur Fd du server
	if (listen(this->serverFd, 128) < 0)
	{
		std::cout << BOLD_RED << "Error while listening" << std::endl;
		perror("listen");
		std::cout << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	// ajout du fd server dans fdListened
	this->fdListened.push_back(pollfd());
	this->fdListened[0].fd = this->serverFd;
	this->fdListened[0].events = POLLIN;
}

void	Server::removeClient(std::string name)
{
	std::vector<Client>::iterator cl = getClient(name);
	std::vector<pollfd>::iterator pfd = getClientByFd(name);
	close(pfd->fd);
	this->fdListened.erase(pfd);
	this->clientList.erase(cl);
	for (std::map<std::string, Channel>::iterator it=this->channelList.begin(); it != this->channelList.end(); it++)
	{
		it->second.removeConnected(name);
		it->second.removeBanned(name);
		it->second.removeOperator(name);
	}
	std::cout << BOLD_RED << "client disconnected : " << name << "\n" << RESET << std::endl;

}

size_t Server::nbOfClientsNotOnline()
{
	size_t total = 0;
	for(std::vector<Client>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		if (it->online == false)
			total++;
	}
	return (total);
}

void Server::removeFirstClientNotOnline()
{
	for(std::vector<Client>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		if (it->online == false)
		{
			this->removeClient(it->userInfos.nickName);
			return;
		}
	}
}

void	Server::removeNotOnlineClient(void)
{
	if (clientList.size() == 0)
		return ;

	while (this->nbOfClientsNotOnline() > 0)
	{
		this->removeFirstClientNotOnline();
	}
}

size_t Server::nbOfEmptyChannels()
{
	size_t total = 0;
	for (std::map<std::string, Channel>::iterator it=this->channelList.begin(); it != this->channelList.end(); it++)
	{
		if (it->second.clientConnected.empty())
			total++;
	}
	return (total);
}

void Server::removeFirstChannelEmpty()
{
	for (std::map<std::string, Channel>::iterator it=this->channelList.begin(); it != this->channelList.end(); it++)
	{
		if (it->second.clientConnected.empty())
		{
			this->channelList.erase(it);
			return;
		}
	}
}

void	Server::removeEmptyChannels()
{
	while (this->nbOfEmptyChannels() > 0)
	{
		this->removeFirstChannelEmpty();
	}
}

void Server::run(int i)
{
	// this->fdListened.reserve(100);

	int pollreturn = -1;
	// while (closeServer == false)
	// {
		std::cout << BOLD_BLUE << "-------------------------------------------------------------------------------------------------" << RESET << std::endl;
		std::cout << BOLD_BLUE << "loop step " << i << BLUE << " (1 loop = " << LISTENING_TIMEOUT/1000 << "s)  Waiting for event ... \U0001F634" << RESET << std::endl;
		pollreturn = poll(&(this->fdListened[0]), this->fdListened.size(), LISTENING_TIMEOUT);
		if(pollreturn < 0)
		{
			return;
			// std::cout << BOLD_RED << "Error with poll()" << std::endl;
			// perror("poll");
			// std::cout << RESET << std::endl;
			// exit(EXIT_FAILURE);
		}
		if(pollreturn > 0)
		{
			std::cout << BOLD_BLUE << "Event detected by poll \n" << RESET;
			this->manage_poll_event();
		}
		if(pollreturn == 0)
		{
			std::cout << BOLD_BLUE << "Poll delay expired \n\n" << RESET;

		}
		this->pingAllClients();
		this->checkInactiveClients();
		this->removeNotOnlineClient();
		this->removeEmptyChannels();
		std::cout << BOLD_BLUE << "Server state at the end of the run loop " << RESET << std::endl;
		this->printState();

		// temporisation pour debug
		//sleep(1); // sleep 1s
		// i++;
	// }
}

void Server::manage_poll_event()
{
	this->fdListened.reserve(this->fdListened.size() + 1);
	for (std::vector<pollfd>::iterator it = this->fdListened.begin(); it != this->fdListened.end(); it++)
	{
		if (it->revents == POLLIN)
		{
			if (it == this->fdListened.begin())
			{
				this->addNewClient();
			}
			else
			{
				this->listen_client(*(this->getClient(it->fd)));
			}
		}
	}
}

void Server::addNewClient()
{
	Client newClient(*this);


	//-- Tentative d'acceptation du nouveau client
	newClient.clientSize = sizeof(newClient.clientAddr);
	std::cout << "check fd" << this->fdListened[0].fd << std::endl;
	if ((newClient.socketFd = accept(this->fdListened[0].fd, (struct sockaddr *)&newClient.clientAddr, &newClient.clientSize)) < 0)
	{
		perror("accept");
		std::cout << BOLD_RED << "Error while accepting connection" << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	this->clientList.push_back(newClient);
	this->fdListened.push_back(pollfd());
	(--this->fdListened.end())->fd = newClient.socketFd;
	(--this->fdListened.end())->events = POLLIN;
}

void Server::listen_client(Client &client)
{
	clear_str(client.buffer, client.bufferSize);
	if(recv(client.socketFd, client.buffer, client.bufferSize - 1, 0) <= 0)
	{
		client.online = false;
		return;
	}
	std::cout << BOLD_RED << "<= " << BOLD_YELLOW << "Message received on fd " << client.socketFd << ": " << BOLD_BLACK "-->" << YELLOW << client.buffer << BOLD_BLACK << "<--" << RESET << std::endl;
	client.cmd += client.buffer;
	replace_rn_by_n(client.cmd);
	while (client.cmd.find("\n") != std::string::npos)
	{
		std::string cc = pop_command(client.cmd);
		std::cout << BOLD_CYAN << "== "<< BOLD_YELLOW << "Launched command: " << BOLD_BLACK << "-->" << YELLOW << cc << BOLD_BLACK << "<--" << RESET << std::endl;
		client.launchCommand(cc);
	}
}

void Server::terminate()
{
	while(this->fdListened.size() > 0)
	{
		std::cout << BOLD_YELLOW << "Close fd " << (*(--(this->fdListened.end()))).fd << RESET << std::endl;
		close((*(--(this->fdListened.end()))).fd); // close last fd
		this->fdListened.pop_back(); // remove last fd
	}
}

bool	Server::isChannelExisting(std::string name)
{
	if (this->channelList.find(name) != this->channelList.end())
		return (true);
	return false;
}

bool	Server::isClientExisting(std::string name)
{
	std::vector<Client>::iterator it;
	for (it = this->clientList.begin(); it != this->clientList.end(); it++)
		{
			if (it->userInfos.nickName == name)
				return (true);
		}
	return (false);
}

void	Server::addChannel(std::string name)
{
	if (this->isChannelExisting(name))
		return;
	Channel chan = Channel(*this, name);
	this->channelList.insert(std::make_pair(name, chan));
}

std::map<std::string, Channel>::iterator Server::getChannel(std::string name)
{
	std::map<std::string, Channel>::iterator it;
	for (it = this->channelList.begin(); it != this->channelList.end(); it++)
	{
		if (it->first == name)
			return (it);
	}
	return (it); // return end si channel non trouvé
}

void Server::printState()
{
	// server parameters
	std::cout << BLUE_PIPE << BOLD_PURPLE << "Server parameters" << RESET << std::endl;
	std::cout << BLUE_PIPE << PURPLE << "  port " << port << " | serverFd " << serverFd << std::endl;
	std::cout << BLUE_PIPE << PURPLE << "  fdListened (fd,events,revents): "<< PURPLE << "[";
	for (std::vector<pollfd>::iterator it=this->fdListened.begin(); it != this->fdListened.end(); it++)
	{
		std::cout << "(" << it->fd << "," << it->events << "," << it->revents << ")";
		if(it + 1 != this->fdListened.end())
			std::cout << ", ";
		else
			std::cout << "]";
	}
	std::cout << RESET << std::endl;

	// Clients parameters
	// list of clients
	std::cout << BLUE_PIPE << BOLD_GREEN << "Clients" << RESET << std::endl;
	std::cout << BLUE_PIPE << GREEN << "  clientList (by nickName): " << "[";
	for (std::vector<Client>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		std::cout << this->getClient(it->socketFd)->userInfos.nickName;
		if(it + 1 != this->clientList.end())
			std::cout << ", ";
	}
	std::cout << "]";
	std::cout << RESET << std::endl;
	// Clients state
	int i = 1;
	for (std::vector<Client>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		std::cout << BLUE_PIPE << GREEN << "  Client " << i << ": ";
		std::cout << "nickName " << it->userInfos.nickName << " | ";
		std::cout << "userName " << it->userInfos.userName << " | ";
		std::cout << "online " << PRINT_BOOL(it->online) << " | ";
		std::cout << "socketFd " << it->socketFd << " | ";
		std::cout << "status " << PRINT_STATUS(it->status) << " (" << it->status << "/4)" << " | " ;
		std::cout << "invisible " << PRINT_BOOL(it->userInfos.invisibleMode) << " | " ;
		std::cout << "operator " << PRINT_BOOL(it->userInfos.operatorMode);
		std::cout << RESET << std::endl;
		i++;
	}

	// Channels parameters
	std::cout << BLUE_PIPE << BOLD_CYAN << "Channels" << RESET << std::endl;
	std::cout << BLUE_PIPE << CYAN << "  channelList (by name): " << "[";
	// list of channel
	for (std::map<std::string, Channel>::iterator it=this->channelList.begin(); it != this->channelList.end(); it++)
	{
		std::cout << it->first;
		if(++it != this->channelList.end())
			std::cout << ", ";
		--it;
	}
	std::cout << "]" << std::endl;

	for (std::map<std::string, Channel>::iterator it=this->channelList.begin(); it != this->channelList.end(); it++)
	{
		// print status in channels
		std::cout << BLUE_PIPE << CYAN << "  Channel " << it->first << ": ";
		std::cout << "name " << it->second.name << " | ";
		std::cout << "topic '" << it->second.topic << "' | ";
		std::cout << "isInviteOnly " << PRINT_BOOL(it->second.isInviteOnly);
		std::cout << RESET << std::endl;


		// print list of connected clients in channels
		std::cout << BLUE_PIPE << CYAN << "  Clients in " << it->first << ": [";
		for (std::set<std::string>::iterator it_client=it->second.clientConnected.begin(); it_client != it->second.clientConnected.end(); it_client++)
		{
			std::cout << *it_client;
			if(++it_client != it->second.clientConnected.end())
				std::cout << ", ";
			--it_client;
		}
		std::cout << "]" << std::endl;

		// print list of operators in channels
		std::cout << BLUE_PIPE << CYAN << "  Clients banned in " << it->first << ": [";
		for (std::set<std::string>::iterator it_client=it->second.clientBanned.begin(); it_client != it->second.clientBanned.end(); it_client++)
		{
			std::cout << *it_client;
			if(++it_client != it->second.clientBanned.end())
				std::cout << ", ";
			--it_client;
		}
		std::cout << "]" << std::endl;

		// print list of operators in channels
		std::cout << BLUE_PIPE << CYAN << "  Operators in " << it->first << ": [";
		for (std::set<std::string>::iterator it_client=it->second.clientOperators.begin(); it_client != it->second.clientOperators.end(); it_client++)
		{
			std::cout << *it_client;
			if(++it_client != it->second.clientOperators.end())
				std::cout << ", ";
			--it_client;
		}
		std::cout << "]" << std::endl;
	}
	std::cout << RESET << std::endl << std::endl;
}

bool operator==( Client& other, const Client& rhs)
{
	return (other.getSocketFd() == rhs.getSocketFd());
}

bool operator==( const Client& other, Client& rhs)
{
	return (other.getSocketFd() == rhs.getSocketFd());
}

bool operator==( const Client& other, const Client& rhs)
{
	return (other.getSocketFd() == rhs.getSocketFd());
}
