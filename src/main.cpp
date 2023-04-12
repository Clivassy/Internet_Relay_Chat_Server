#include "main.hpp"
#include <signal.h>

int main(int ac, char **av)
{
	Server server;
	parsing_arguments(ac, av, server);
	server.init();
	server.run();
	server.terminate();
}

/*bool closeServer = false;

void	handler(int signal)
{
	(void)signal;
	closeServer = true;
}

int main(int ac, char **av)
{
	Server server;
	parsing_arguments(ac, av, server);
	signal(SIGINT, handler);
	server.init();
	while (closeServer == false)
		server.run();
	server.terminate();
}*/
