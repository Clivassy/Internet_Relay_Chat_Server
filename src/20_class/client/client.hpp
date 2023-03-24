#ifndef CLIENT_HPP
# define CLIENT_HPP

//#include "../main.hpp"
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
# include  <sstream>
# include "../../10_tools/colors.hpp"
# include "../../10_tools/errors.hpp"
# include "../../10_tools/utils.hpp"
# include "../server.hpp"
# include "../channel.hpp"

class Server;
class Channel;

# define BUFFER_SIZE 1024
# define WAITING 0
# define COMING 1
# define REGISTERED 2
# define BAD_PASSWORD 3
# define CONNECTED 4

class Client {

    struct User {
        std::string nickName;
        std::string userName;
        std::string hostName;
        std::string serverName;
        std::string realName;
        std::string userMessage;


    };
    public:
        Client(Server& serv);
		Client& operator=(const Client&other)
		{
			(void)other;
			return (*this);
		}
        ~Client(); 
		
		// Command
		bool	launchCommand(std::string command);
		void	sendMessage(std::string str);
		void	sendOtherClient(std::string str);
		bool	cmdCAP(std::vector<std::string> &cmd);
		bool	cmdPASS(std::vector<std::string> &cmd);
		bool	cmdUSER(std::vector<std::string> &cmd);
		bool	cmdNICK(std::vector<std::string> &cmd);
		bool	cmdPING(std::vector<std::string> &cmd);
		bool	cmdOPER(std::vector<std::string> &cmd);
		bool	cmdQUIT(std::vector<std::string> &cmd);
		bool	cmdJOIN(std::vector<std::string> &cmd);
		bool	cmdPART(std::vector<std::string> &cmd);
		bool	cmdINVITE(std::vector<std::string> &cmd);
		bool	cmdKICK(std::vector<std::string> &cmd);
		bool	cmdMODE(std::vector<std::string> &cmd);
		bool	cmdPRIVMSG(std::vector<std::string> &cmd);
		bool	cmdNOTICE(std::vector<std::string> &cmd);
		bool	cmdWHOIS(std::vector<std::string> &cmd);

        void    			fillDataUser( void );
        void    			sendResponse( void );
        bool    			getNickName( std::string toSplit );
        bool    			getUserInfos( std::string toSplit);
		std::string 	    getPassword( std::string toSplit );
		std::string			getPrefix( void );
		void				errorAuthentification ( void );
		void				errorPassword( void );

        //-- Getters
        //std::string    getUserName( void );
        //std::string    gethostName( void );
        //std::string    getserverName( void );
        //std::string    getRealName( void );
        //std::string    getUserMessage( void );

        //-- Setters
        //void    setUserName( std::string userName );
        //void    setHostName( std::string userName );
        //void    setServerName( std::string userName );
        //void    setRealName( std::string userName );
        //void    setUserMessage( std::string userName );

    // protected:
		Server&						server;
        int							socketFd;
        struct sockaddr_in          clientAddr;
        socklen_t                   clientSize;
        User                        userInfos;
		const int			        bufferSize;
		char				        buffer[BUFFER_SIZE];
   		std::string                 authentification;
        std::string                 cmd;
		int							status;
		std::vector<std::string>	authentificationCmd;

};
    std::string                 removeLines(std::string);
    //std::vector<std::string>    splitCommand(std::string toSplit, char sep);


#endif
