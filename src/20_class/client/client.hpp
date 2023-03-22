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
# include "../../10_tools/utils.hpp"
# include "../server.hpp"

class Server;

# define BUFFER_SIZE 1024


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
		bool	launchCommand(void);
		void	sendMessage(std::string str);
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
		bool	    		getPassword( std::string toSplit );
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
        int socketFd;
        struct sockaddr_in          clientAddr;
        socklen_t                   clientSize;
        User                        userInfos;
		const int			        bufferSize;
		char				        buffer[BUFFER_SIZE];
   		std::string                 authentification;
        std::string                 cmd;
		bool                        isAuthentified;
		bool                        isConnected;
		bool						isOperator;
		bool						isValidPassword;
		std::vector<std::string>	authentificationCmd;

	// Authentication
		bool CAP_LS;
		bool Password;
		bool NICK;
		bool USER;

};
    std::string                 removeLines(std::string);
    //std::vector<std::string>    splitCommand(std::string toSplit, char sep);


#endif
