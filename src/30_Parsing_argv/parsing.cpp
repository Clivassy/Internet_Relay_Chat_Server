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
	int i = 0;
	while (av[1][i])
	{
		if (!isdigit(av[1][i]))
		{
			std::cout << BOLD_RED << "Errror : Port must be all digit" << RESET << std::endl;
			exit(EXIT_FAILURE);
		}
		i++;
	}

	//  - validité passsword
	i = 0;
	while (av[2][i])
	{
		if (av[2][i] == ' ' || av[2][i] == ':')
		{
			std::cout << BOLD_RED << "Errror : Password can't have ' '  or ':' caracteres" << RESET << std::endl;
			exit(EXIT_FAILURE);
		}
		i++;
	}

	server.set_port(atoi(av[1]));
	server.set_password(av[2]);
}
