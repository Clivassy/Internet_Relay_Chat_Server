#include "server.hpp"
// TBD passer class dans fichiers .cpp et .hpp a part


Server::Server()
//bufferSize(BUFFER_SIZE)
{

}

Client& Server::getClient(int fd)
{
	for (std::vector<Client>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
		{
			if (it->socketFd == fd)
				return (*it);
		}
	std::cout << "fd non trouve dans getclient " << std::endl;
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

void Server::run()
{
	this->fdListened.reserve(100);

	int i = 0; // limite pour eviter de kill process en debug, TBD mettre loop infinie
	// TBD redefinir ctrl+C pour close les fd (evite de bloquer les ports en debug)
	
	int pollreturn = -1;
	while (i < 7)
	{
		std::cout << BOLD_BLUE << "loop step " << i << RESET << std::endl;
		std::cout << "Ecoute pollfd " << RESET << std::endl;
		pollreturn = poll(&(this->fdListened[0]), this->fdListened.size(), LISTENING_TIMEOUT);
		for (std::vector<pollfd>::iterator it = this->fdListened.begin(); it != this->fdListened.end(); it++)
		{
			std::cout << "it->revents: " << it->fd << " / " << it->revents << std::endl;

		}
		if(pollreturn < 0)
		{
			std::cout << BOLD_RED << "Error with poll()" << std::endl;
			perror("poll");
			std::cout << RESET << std::endl;
			exit(EXIT_FAILURE);
		}
		if(pollreturn > 0)
		{
			this->manage_poll_event();
		}
		if(pollreturn == 0) // TBD bloc pour debug, mais a supprimer car inutile a la fin
		{
			std::cout << "delay expired" << std::endl;
		}

		// temporisation pour debug (TBD a enlever a la fin)
		sleep(1); // sleep 1s
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

				// Fonction Julia pour ajout nouveau client
					//while(responseSize > 0)
					//{
					//	responseSize = recv(this->fdListened[0].fd, this->buffer, this->bufferSize -1, 0);
						
					//	std::cout << "responseSize: " << responseSize << RESET << std::endl;
					//	std::cout << "buffer: " << buffer << RESET << std::endl;
					//}

			}
			else
			{
				this->listen_client(this->getClient(it->fd));
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
	//this->fdListened[1].fd = newClient.socketFd;
	(--this->fdListened.end())->fd = newClient.socketFd;
	(--this->fdListened.end())->events = POLLIN; // event attendu = POLLIN
	//this->authentication(*(--this->clientList.end()));
}



void Server::listen_client(Client &client)
{
	std::cout << "listen\n";
	if (!client.is_authentified)
	{
		recv(client.socketFd, client.buffer, client.bufferSize - 1, 0); // TBD voir si flag O_NONBLOCK
		client.authentification += client.buffer;

		std::cout << client.authentification << std::endl;
	
		client.fillDataUser();
		client.sendResponse();

		client.authentification.clear();
	}
	else
	{
		// TBD faire loop poour recuperer command complete avant envoie partie Arzu
		// qd cmd complet copier dans client/cmd et envoyer fct
		recv(client.socketFd, client.buffer, client.bufferSize - 1, 0);
		client.cmd += client.buffer;
		std::cout << client.cmd << std::endl;
		client.cmd.clear();

	}



}

//void Server::listen_client(pollfd& client_pollfd)
//{
//	// ecoute client et lancement commande
//}

void Server::terminate()
{
	//close(this->serverFd);
	while(this->fdListened.size() > 0)
	{
		std::cout << BOLD_YELLOW << "Close fd " << (*(--(this->fdListened.end()))).fd << RESET << std::endl;
		close((*(--(this->fdListened.end()))).fd); // close last fd
		this->fdListened.pop_back(); // remove last fd
	}
}



