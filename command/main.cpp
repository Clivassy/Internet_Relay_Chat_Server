#include <iostream>
#include "test.hpp"

bool	check_command(std::string &s, std::string &pw, int *cnct);

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	int *cnct = 0;
	std::string s;
	std::string pw = "bouh";
	{
		std::cout << "\n ---------- PASS ----------" << std::endl; 
		s = "PASS";
		std::cout << std::endl << "check : " << s << std::endl;
		check_command(s, pw, cnct);
		s = "PASS bouh";
		std::cout << std::endl << "check : " << s << std::endl;
		check_command(s, pw, cnct);
		s = "PASS prout";
		std::cout << std::endl << "check : " << s << std::endl;
		check_command(s, pw, cnct);
	}
	{
		std::cout << "\n ---------- NICK ----------" << std::endl; 
		s = "NICK";
		std::cout << std::endl << "check : " << s << std::endl;
		check_command(s, pw, cnct);
		s = "NICK bouh";
		std::cout << std::endl << "check : " << s << std::endl;
		check_command(s, pw, cnct);
	}
//	std::cout << std::endl << "check : " << s << std::endl;
//	check_command(s, pw, cnct);
	return (0);
}
