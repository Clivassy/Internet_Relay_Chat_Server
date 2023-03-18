#ifndef MAIN_HPP
# define MAIN_HPP

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



#include "20_class/server.hpp"
//#include "20_class/client.hpp"


//-- Personnal Libraries 
# include "10_tools/colors.hpp"

// proto class
class Server;


// parsing_argv
void parsing_arguments(int ac, char **av, Server& server);

#endif 
