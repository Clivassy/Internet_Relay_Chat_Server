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
# include "../10_tools/colors.hpp"




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
        Client();
        ~Client(); 
        void    fillDataUser(Client &client, std::string token, char sep);
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
        int socketFd;
        struct sockaddr_in clientAddr;
        socklen_t clientSize;
        User userInfos;
        std::vector<std::string> cmd;
};
#endif
