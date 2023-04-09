#include "server.hpp"
// TBD passer class dans fichiers .cpp et .hpp a part


Server::Server()
//bufferSize(BUFFER_SIZE)
{

}

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
	std::cout << BOLD_RED << "user non trouve dans getclient -> /!\\ pour suite execution (return getClient non valid)" << RESET << std::endl;
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
	std::cout << BOLD_RED << "user non trouve dans getclientByFd -> /!\\ pour suite execution (return getClientByFd non valid)" << RESET << std::endl;
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
			this->killclient(it->userInfos.nickName, "No response to ping in time");
		}
	}
}

void Server::killclient(std::string name, std::string reason)
{
	(void)reason;
	// TBD refaire kill du cient (suppression dans fdListened, clientList et de l'objet client si c'est pas auto)
	// TBD attention, voir avant pourquoi irssi ne repond pas au 1er ping
	std::cout << BOLD_RED << "TBD: fonction to kill Client: " << name << ", because: " << reason << RESET << std::endl;
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

bool operator==( Client& other, const Client& rhs) 
{
    return (other.socketFd == rhs.socketFd);
}

bool operator==( const Client& other, Client& rhs) 
{
    return (other.socketFd == rhs.socketFd);
}

bool operator==( const Client& other, const Client& rhs) 
{
    return (other.socketFd == rhs.socketFd);
}

void Server::init()
{
	//memset(this->buffer, 0, bufferSize); // TBD voir si il faut pas remplir de bufferS0ze * sizeof(int)
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
	if (listen(this->serverFd, 5) < 0) // TBD pourquoi 5 ?
	{
		std::cout << BOLD_RED << "Error while listening" << std::endl;
		perror("listen");
		std::cout << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	// ajout du fd server dans fdListened
	this->fdListened.push_back(pollfd());
	this->fdListened[0].fd = this->serverFd;
	this->fdListened[0].events = POLLIN; // event attendu = POLLIN

	// TBD fonction pour print les attributs du server
}

void	Server::removeClient(std::string name)
{
	this->printState();
	print_vector_client(this->clientList);
	std::cout << "it: " << this->clientList.begin()->userInfos.nickName << std::endl;
	std::cout << "it: " << (++this->clientList.begin())->userInfos.nickName << std::endl;
	std::cout << "it: " << (--this->clientList.end())->userInfos.nickName << std::endl;
	this->clientList.erase(this->clientList.begin());
	this->printState();
	print_vector_client(this->clientList);

//	std::cout << "name is removeClient " << name << std::endl;
	std::vector<Client>::iterator cl = getClient(name);
	std::vector<pollfd>::iterator pfd = getClientByFd(name);
	std::cout << "name: " << name << std::endl;
	std::cout << "client name it: " << cl->userInfos.nickName << std::endl;
//	std::cout << "name is clientList " << cl->userInfos.nickName << std::endl;
//	std::cout << "name is fdListened " << pfd->fd << cl->socketFd << std::endl;
	close(pfd->fd);
	this->fdListened.erase(getClientByFd(name));
	this->printState();
	std::cout << "name: " << name << std::endl;
	std::cout << "client name it: " << cl->userInfos.nickName << std::endl;
	this->clientList.erase(this->clientList.begin());
	this->printState();

	
	for (std::map<std::string, Channel>::iterator it=this->channelList.begin(); it != this->channelList.end(); it++)
	{
		it->second.removeConnected(it->second.name);
		it->second.removeBanned(it->second.name);
		it->second.removeOperator(it->second.name);
	}
}

// TBD peut servir si removeNotOnlineClient() ne marche pas TBD fct a checker
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

// TBD en cours
void	Server::removeNotOnlineClient(void)
{
	if (clientList.size() == 0)
		return ;

	while (this->nbOfClientsNotOnline() > 0)
	{
		this->removeFirstClientNotOnline();
	}
	//for (std::vector<Client>::iterator it = clientList.begin(); it != clientList.end(); it++)
	//{
	//	if (it->online == false)
	//	{
	//		this->removeClient((*(it)).userInfos.nickName);
	//	}
	//}	
}

void Server::run()
{
	this->fdListened.reserve(100);

	int i = 0; // limite pour eviter de kill process en debug, TBD mettre loop infinie
	// TBD redefinir ctrl+C pour close les fd (evite de bloquer les ports en debug)

	// tmp pour debug
	//Client newClient1(*this);
	//newClient1.userInfos.nickName = "usr1";
	//this->clientList.push_back(newClient1);
	//Client newClient2(*this);
	//newClient2.userInfos.nickName = "usr2";
	//this->clientList.push_back(newClient2);
	//Client newClient3(*this);
	//newClient3.userInfos.nickName = "usr3";
	//this->clientList.push_back(newClient3);
	//print_vector_client(this->clientList);
	//this->clientList.erase(this->clientList.begin());
	//print_vector_client(this->clientList);

	//objTest objTest1;
	//objTest1.var = "usr1";
	//this->clientList_objtest.push_back(objTest1);
	//objTest objTest2;
	//objTest2.var = "usr2";
	//this->clientList_objtest.push_back(objTest2);
	//objTest objTest3;
	//objTest3.var = "usr3";
	//this->clientList_objtest.push_back(objTest3);
	//std::cout << "vector: " << clientList_objtest[0].var << " - " << clientList_objtest[1].var << " - " << clientList_objtest[2].var << std::endl;
	//this->clientList_objtest.erase(this->clientList_objtest.begin());
	//std::cout << "vector: " << clientList_objtest[0].var << " - " << clientList_objtest[1].var<< std::endl;


	//std::string s1("s1");
	//this->clientList_test.push_back(s1);
	//std::string s2("s2");
	//this->clientList_test.push_back(s2);
	//std::string s3("s3");
	//this->clientList_test.push_back(s3);
	//print_vector_str(this->clientList_test);
	//this->clientList_test.erase(this->clientList_test.begin());
	//print_vector_str(this->clientList_test);


	//std::cout << "fin test\n";
	//sleep(10);
	// fin tmp
	
	int pollreturn = -1;
	while (1)
	{
		// fin tmp
		std::cout << BOLD_BLUE << "-------------------------------------------------------------------------------------------------" << RESET << std::endl;
		std::cout << BOLD_BLUE << "loop step " << i << BLUE << " (1 loop = " << LISTENING_TIMEOUT/1000 << "s)  Waiting for event ... \U0001F634" << RESET << std::endl;
		pollreturn = poll(&(this->fdListened[0]), this->fdListened.size(), LISTENING_TIMEOUT);
		if(pollreturn < 0)
		{
			std::cout << BOLD_RED << "Error with poll()" << std::endl;
			perror("poll");
			std::cout << RESET << std::endl;
			exit(EXIT_FAILURE);
		}
		if(pollreturn > 0)
		{
			std::cout << BOLD_BLUE << "Event detected by poll \n" << RESET;
			this->manage_poll_event();
		}
		if(pollreturn == 0) // TBD bloc pour debug, mais a supprimer car inutile a la fin
		{
			std::cout << BOLD_BLUE << "Poll delay expired \n\n" << RESET;

		}
		// rm client not on line
		this->pingAllClients(); // TBD ajout check reponse (flag lastPong dans client + deco si delay depassé)
		this->checkInactiveClients();
		this->removeNotOnlineClient();
		std::cout << BOLD_BLUE << "Server state at the end of the run loop " << RESET << std::endl;
		this->printState();


		// temporisation pour debug (TBD a enlever a la fin)
		//sleep(1); // sleep 1s
		i++;
	}
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
	// TBD partie a supprimer quand on aura removeClientWithNegativeRevent()
	//std::vector<pollfd>::iterator ite = this->fdListened.begin();
	//for (size_t i = 0; i < fdListened.size(); i++, ite++)
	//{
	//	if(ite->revents == -1)
	//	{
	//		close(ite->fd);
	//		this->fdListened.erase(ite);

	//		std::vector<Client>::iterator it  = std::find(this->clientList.begin(), this->clientList.end(), *(this->getClient(ite->fd)) );
	//		if (it != clientList.end())
    //		{	
	//			this->clientList.erase(it);
	//		}
	//	}
	//}
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
	//this->fdListened[1].fd = newClient.socketFd;
	(--this->fdListened.end())->fd = newClient.socketFd;
	(--this->fdListened.end())->events = POLLIN; // event attendu = POLLIN
	//this->authentication(*(--this->clientList.end()));
}

void Server::listen_client(Client &client)
{
	clear_str(client.buffer, client.bufferSize);
	if(recv(client.socketFd, client.buffer, client.bufferSize - 1, 0) <= 0)
	{
		std::cout << BOLD_RED << "Error on client " << client.userInfos.nickName << "\n" << RESET << std::endl;
		//this->getPollfd(client.socketFd)->revents = -1; // passage du revent pour que ce client soit kill (le kill ne peut pas etre fait ici pour ne pas invalider les iterateurs dans la boucle appelant listen_client)
		client.online = false; // passage du revent pour que ce client soit kill (le kill ne peut pas etre fait ici pour ne pas invalider les iterateurs dans la boucle appelant listen_client)
		return;
		// Ajout flag pour kill client et return (client.revents = -1)
		// le kill doit etre fait plus tard en dehors de la boucle appelant listen_client
		//this->killclient(client.userInfos.nickName, "recv return <= 0");
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
	if (this->channelList.count(name) >= 1)
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
	Channel chan = Channel(*this, name); // TBD initialiser les attributs du channel si on en ajoute
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
	
	
	//return((this->channelList.find(name))->second);
}

void Server::printState()
{
	// server parameters
	std::cout << BLUE_PIPE << BOLD_PURPLE << "Server parameters" << RESET << std::endl;
	std::cout << BLUE_PIPE << PURPLE << "  port " << port << " | socketFd " << socketFd << " | serverFd " << serverFd << std::endl;
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
//		# define COMING 1
//# define REGISTERED 2
//# define BAD_PASSWORD 3
//# define CONNECTED 4
		//std::cout << "isAuthentified " << PRINT_BOOL(it->isAuthentified) << " | ";
		//std::cout << "isConnected " << PRINT_BOOL(it->isConnected) << " | ";
		//std::cout << "isOperator " << PRINT_BOOL(it->isOperator) << " | ";
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


