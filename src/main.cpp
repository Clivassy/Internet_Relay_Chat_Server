#include "main.hpp"

int main(int ac, char **av)
{
	Server server;
	parsing_arguments(ac, av, server);
	server.init();
	server.run();
	server.terminate();

}
