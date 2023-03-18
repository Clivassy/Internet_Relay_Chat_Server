#include <iostream>
#include "test.hpp"
# include "../src/20_class/client.hpp"

class Client;
int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	Client clt;
	int cnct = 0;
	std::string s;
	std::string pw = "bouh";
	{
		std::cout << "\n ---------- PASS ----------" << std::endl;
		s = "PASS";
		std::cout << std::endl << "check : " << s << std::endl;
		clt.launchCommand(s);
		s = "PASS bouh";
		std::cout << std::endl << "check : " << s << std::endl;
		clt.launchCommand(s);
		s = "PASS prout";
		std::cout << std::endl << "check : " << s << std::endl;
		clt.launchCommand(s);
	}
	{
		std::cout << "\n ---------- NICK ----------" << std::endl;
		s = "NICK";
		std::cout << std::endl << "check : " << s << std::endl;
		clt.launchCommand(s);
		s = "NICK bouh";
		std::cout << std::endl << "check : " << s << std::endl;
		clt.launchCommand(s);
	}
//	std::cout << std::endl << "check : " << s << std::endl;
//	launchCommand(s, pw, cnct);
	return (0);
}
