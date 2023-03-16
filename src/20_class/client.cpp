#include "client.hpp"

Client::Client():
bufferSize(BUFFER_SIZE), is_authentified(false)
{
}

Client::~Client(){ }


/*
void    Client::fillDataUser(Client &client, std::string token, char sep)
{
	std::stringstream toSplit(token);
	std::string userNick;
	std::vector<std::string> userNickName;

	while (getline(toSplit, userNick, ' '))
	{
		userNickName.push_back(userNick);
	}
    // switch case 

	client.userInfos.userName = userNickName[1];
}
*/
//-- Getters
//std::string    getUserName( void ) {}
//std::string    gethostName( void ) {}
//std::string    getserverName( void ){}
//std::string    getRealName( void ) {}
//std::string    getUserMessage( void ) {}

//-- Setters
//void    setUserName( std::string userName ) {}
//void    setHostName( std::string userName ) {}
//void    setServerName( std::string userName ) {}
//void    setRealName( std::string userName ) {}
//void    setUserMessage( std::string userName ) {}
