#include <string>
# include <iostream>
# include <string>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <netdb.h>
# include <poll.h>
# include <vector>
# include <map>

#include "utils.cpp" 
#include "colors.hpp" 

int main()
{
	std::string str = "abcd\r\n123\r\n56789";
	std::cout << BOLD_BLUE << "str before:\n" << RESET << str << std::endl;

	std::cout << BOLD_BLUE << "pop return:\n" << RESET << pop_command(str) << std::endl;
	std::cout << BOLD_BLUE << "str after:\n" << RESET << str << std::endl;

	std::cout << BOLD_BLUE << "pop return:\n" << RESET << pop_command(str) << std::endl;
	std::cout << BOLD_BLUE << "str after:\n" << RESET << str << std::endl;

	std::cout << BOLD_BLUE << "pop return:\n" << RESET << pop_command(str) << std::endl;
	std::cout << BOLD_BLUE << "str after:\n" << RESET << str << std::endl;

	std::cout << BOLD_BLUE << "pop return:\n" << RESET << pop_command(str) << std::endl;
	std::cout << BOLD_BLUE << "str after:\n" << RESET << str << std::endl;

	std::cout << BOLD_BLUE << "pop return:\n" << RESET << pop_command(str) << std::endl;
	std::cout << BOLD_BLUE << "str after:\n" << RESET << str << std::endl;
	
	return (0);
}
