#ifndef CLIENT_HPP
# define CLIENT_HPP

//#include "../main.hpp"
# include <iostream>
# include <string>
# include <string.h>
# include <sstream>
# include <iostream>
# include <iomanip>
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
# include <ctime>
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
# define PRINT_STATUS(value) ((value == COMING) ? "Coming \U0000231B" : ((value == REGISTERED) ? "Registered \U000023F3" : ((value == BAD_PASSWORD) ? "Bad password \U0000274C" : ((value == CONNECTED) ? "Connected \U00002705" : "Unknown status"))))


class Client {

    struct User {
        std::string nickName;
        std::string userName;
        std::string hostName;
        std::string serverName;
        std::string realName;
        std::string userMessage;
        bool        invisibleMode;
        bool        operatorMode;

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
		bool	cmdPONG(std::vector<std::string> &cmd);
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

		void				ping();
		std::string			getPrefix( void );
		void				deconnectClient( void );
       
        //-- commandMODE.cpp
        bool                parsingErrorChannel(std::vector<std::string> cmd);
        int		            updateUserModes(char sign, char mode);
        int                 updateChannelModes(char sign, char mode, std::string user);
        int		            parseModes(std::string modes, int modeType, std::string user);
        bool	            addUserMode(std::vector<std::string> cmd);
        bool	            addChannelMode(std::vector<std::string> cmd);
        std::string		    getChannelName(std::vector<std::string> cmd);

        //-- commandeNICK.cpp
        bool	            checkDoubleNICK(std::string cmd, std::string errMsg);
        bool	            isClientInServer(std::string nickname);

        //-- commandeKICK.cpp
        bool                isValidParsingKICK(std::vector<std::string> &cmd);

        //-- commandeINVITE.cpp
        bool                isValidParsingINVITE(std::vector<std::string> &cmd);
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
        std::string                 cmd;
		int							status;
		bool						hasNick;
		time_t						lastPingSent;
		time_t						lastPongReceived;


};
    //std::vector<std::string>    splitCommand(std::string toSplit, char sep);


#endif
