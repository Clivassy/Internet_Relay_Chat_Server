#include "main.hpp"
#include <signal.h>

/*int main(int ac, char **av)
{
	Server server;
	parsing_arguments(ac, av, server);
	server.init();
	server.run();
	server.terminate();
}*/

bool closeServer;

void	handler(int signal)
{
	(void)signal;
	closeServer = true;
}

int main(int ac, char **av)
{
	closeServer = false;
	Server server;
	parsing_arguments(ac, av, server);
	signal(SIGINT, handler);
	server.init();
	int i = 0;
	while (!closeServer)
	{
		server.run(i);
		i++;
	}
	server.terminate();
}
