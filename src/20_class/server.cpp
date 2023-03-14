#include "server.hpp"
// TBD passer class dans fichiers .cpp et .hpp a part


Server::Server():
bufferSize(BUFFER_SIZE)
{

}


void Server::set_port(int port)
{
	this->port = port; 
}

void Server::set_password(std::string password)
{
	this->password = password;
}

void Server::init()
{
	memset(this->buffer, 0, bufferSize); // TBD voir si il faut pas remplir de bufferS0ze * sizeof(int)
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
	// ajout du fd server dans fd_listened
	this->fd_listened.push_back(pollfd());
	this->fd_listened[0].fd = this->serverFd;
	this->fd_listened[0].events = POLLIN; // event attendu = POLLIN

	// TBD fonction pour print les attributs du server
}

void Server::run()
{
	int i = 0; // limite pour eviter de kill process en debug, TBD mettre loop infinie
	// TBD redefinir ctrl+C pour close les fd (evite de bloquer les ports en debug)
	
	//std::vector<pollfd> fd_listened;
	//fd_listened.push_back(pollfd());
	//fd_listened[0].fd = this->serverFd;
	//fd_listened[0].events = POLLIN;
	int pollreturn = -1;
	while (i < 3)
	{
		std::cout << BOLD_BLUE << "loop step " << i << RESET << std::endl;
		std::cout << "Ecoute pollfd " << RESET << std::endl;
		pollreturn = poll(&(this->fd_listened[0]), this->fd_listened.size(), LISTENING_TIMEOUT);
		if(pollreturn < 0)
		{
			std::cout << BOLD_RED << "Error with poll()" << std::endl;
			perror("poll");
			std::cout << RESET << std::endl;
			exit(EXIT_FAILURE);
		}
		if(pollreturn > 0)
		{
			std::cout << "poll event detected: " << pollreturn << RESET << std::endl;
			std::cout << "poll return: " << pollreturn << RESET << std::endl;
			std::cout << "poll events: " << fd_listened[0].events << RESET << std::endl;
			std::cout << "poll revents: " << fd_listened[0].revents << RESET << std::endl;
			// call fonction pour gérer event (julia)

		}
		if(pollreturn == 0) // TBD bloc pour debug, mais inutile a la fin
		{
			std::cout << "delay expired" << std::endl;
		}

		// ecoute nouvelle connection (accept) -> ajout client si nouvelle connexion entrante

		// ecoute des clients (poll) -> lancement commande si message recu
		// parsing command
		// execution command

		// temporisation pour debug (TBD a enlever a la fin)
		sleep(1); // sleep 1s
		i++;
	}
}

void Server::terminate()
{
	//close(this->serverFd);
	while(this->fd_listened.size() > 0)
	{
		std::cout << BOLD_YELLOW << "Close fd " << (*(--(this->fd_listened.end()))).fd << RESET << std::endl;
		close((*(--(this->fd_listened.end()))).fd); // close last fd
		this->fd_listened.pop_back(); // remove last fd
	}
}



