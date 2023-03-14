#include "../main.hpp"

void parsing_arguments(int ac, char **av, Server& server)
{
	// av[1] == port #include "server.hpp"

	// av[2] == password
	if (ac != 3)
	{
		std::cout << BOLD_RED << "Wrong input arguments" << std::endl;
		std::cout << "Ex: ./ircserv [port] [password]" << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	// TBD a ajouter
	//  - validité port
	//  - validité passsword

	server.set_port(atoi(av[1]));
	server.set_password(av[2]);
}
