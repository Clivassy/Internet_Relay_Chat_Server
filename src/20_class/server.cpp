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
	if (listen(serverFd, 5) < 0) // TBD pourquoi 5 ?
	{
		std::cout << BOLD_RED << "Error while listening" << std::endl;
		perror("listen");
		std::cout << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	// TBD fonction pour print les attributs du server
}

void Server::run()
{
	int i = 0; // limite pour eviter de kill process en debug, TBD mettre loop infinie
	// TBD redefinir ctrl+C pour close les fd (evite de bloquer les ports en debug)
	
	std::vector<pollfd> fd_listened;
	fd_listened.push_back(pollfd());
	fd_listened[0].fd = this->serverFd;
	fd_listened[0].events = POLLIN;

	pollfd test_pollfd[1];
	test_pollfd[0].fd = this->serverFd;
	test_pollfd[0].events = POLLIN;

	while (i < 3)
	{
		std::cout << BOLD_BLUE << "loop step " << i << RESET << std::endl;
		std::cout << "Ecoute fd_listened[0] = nouveau client " << RESET << std::endl;
		int pollreturn = poll(&fd_listened[0], fd_listened.size(), LISTENING_TIMEOUT);
		std::cout << "poll return: " << pollreturn << RESET << std::endl;
		std::cout << "poll events: " << fd_listened[0].events << RESET << std::endl;
		std::cout << "poll revents: " << fd_listened[0].revents << RESET << std::endl;

	//	struct sockaddr_in clientAddr;
	//	socklen_t clientSize = sizeof(clientAddr);
	//	int clientFd;
	//	//listen(this->serverFd, 5);
	//	std::cout << "this->serverFd: " << this->serverFd << std::endl;
	//	if ((clientFd = accept(this->serverFd,(struct sockaddr *)&clientAddr, &clientSize)) < 0)
    //{
    //    std::cout << BOLD_RED << "Error while accepting connection" << RESET << std::endl;
    //    perror(NULL);
	//	exit(EXIT_FAILURE);     
    //}
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
	close(this->serverFd);
	// close (this->clientFd); // TBD close les fd clients
}



