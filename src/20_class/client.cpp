#include "client.hpp"

Client::Client(Server& serv):
server(serv), bufferSize(BUFFER_SIZE), is_authentified(false), is_connected(false), isOperator(false)
{

}

Client::~Client(){ }

void    Client::sendResponse( void )
{
	this->userInfos.userMessage = 
	":" + this->userInfos.nickName 
	+ "!" 
	+ this->userInfos.userName + "@" + this->userInfos.hostName
	+ " 001 " 
	+ this->userInfos.nickName 
	+ " :Welcome to the Internet Relay Network " 
	+ this->userInfos.nickName
	+ "!" 
	+ this->userInfos.userName 
	+ "@" + this->userInfos.serverName
	+ "\r\n" ;

	std::cout << RED << this->userInfos.userMessage << std::endl;

	send(this->socketFd, this->userInfos.userMessage.c_str(), this->userInfos.userMessage.size(), 0);
	// ADD protection en cas d'échec d'authentification
}



std::string		removeLines( std::string toSplit )
{
	for (size_t i = 0; i < toSplit.size(); i++) {
        if (toSplit[i] == '\n') {
            toSplit.erase(i, 1);
        }
    }
	return(toSplit);
}

bool    Client::getPassword( std::string toSplit )
{
	std::vector<std::string> result;

	toSplit = removeLines(toSplit);
	std::cout << BOLD_YELLOW <<  toSplit << RESET << std::endl;
	result = split(toSplit, ' ');
	if (result[0].compare("PASS") == 0 && result.size() == 2)
	{
		std::cout << GREEN << "OKAY" << std::endl;
		this->server.password = result[1];
		// stocker ici le password
		// check le password
		std::cout << BOLD_YELLOW <<  result[1] << RESET << std::endl;
		return(true);
	}
	return (false);
}

bool    Client::getNickName( std::string toSplit )
{
	std::vector<std::string> result;

	toSplit = removeLines(toSplit);
	std::cout << BOLD_YELLOW <<  toSplit << RESET << std::endl;
	result = split(toSplit, ' ');
	/*std::cout << GREEN << "|" << result[0] << "|" << std::endl;
	std::cout << "|" << result[1] << "|" << std::endl;
	std::cout << "size = " << result.size() << RESET << std::endl;*/
	if (result[0].compare("NICK") == 0 && result.size() == 2)
	{
		std::cout << GREEN << "OKAY" << std::endl;
		this->userInfos.nickName = result[1];
		std::cout << GREEN << this->userInfos.nickName << std::endl;
		return(true);
	}
	return (false);
}

bool    Client::getUserInfos( std::string toSplit )
{
	std::vector<std::string> result;
	std::vector<std::string> username;

	toSplit = removeLines(toSplit);
	std::cout << BOLD_YELLOW <<  toSplit << RESET << std::endl;
	result = split(toSplit, ':');
	std::cout << GREEN << "|" << result[0] << "|" << std::endl;
	std::cout << "|" << result[1] << "|" << std::endl;
	std::cout << "size = " << result.size() << RESET << std::endl;
	if (result.size() == 2)
	{
		username = split(result[0], ' ');
		if (username[0].compare("USER") == 0)
		{
			if (username.size() == 4)
			{
				this->userInfos.userName = username[1];
				this->userInfos.hostName = username[2];
				this->userInfos.serverName = username[3];
			}
			this->userInfos.realName = result[1];
			// DEBEUG
			std::cout << GREEN << "userName : " << this->userInfos.userName << std::endl;
			std::cout << GREEN << "HostName : " << this->userInfos.hostName<< std::endl;
			std::cout << GREEN << "ServerName : " << this->userInfos.serverName<< std::endl;
			std::cout << GREEN << "RealName : " << this->userInfos.realName << std::endl;
			
			return(true);
		}
	}
	return (false);
}

void    Client::fillDataUser( void )
{
	std::vector<std::string> informations;
	std::vector<std::string> result;

	informations = split(this->authentification, '\r');
	std::cout << RED << this->authentification << "size = " << informations.size() << RESET << std::endl;

	if (informations.size() == 5)
	{
		std::cout<< "BEGING AUTHENTICATION\n" << std::endl;
		result = split(informations[0], ' ');
		//std::cout << BOLD_PURPLE  << informations[0] << std::endl;
		//std::cout << BOLD_PURPLE  << informations[1] << std::endl;
		//std::cout << BOLD_PURPLE  << informations[2] << std::endl;
		//std::cout << BOLD_PURPLE  << informations[3] << std::endl;
		if (result[0].compare("CAP") == 0 && result[1].compare("LS") == 0 && result.size() == 2)
		{
			std::cout << BOLD_RED << "OK\n";
			std::cout << informations[1] << std::endl;
			if (this->getPassword(informations[1]) == true)
			{
				std::cout << "Password\n";
				if (this->getNickName(informations[2]) == true)
					{
						if (this->getUserInfos(informations[3]) == true)
							is_authentified = true;	
					}
			}					
			return;
		}
		else
			std::cout << BOLD_RED << "Error, cannot authentitate user\n";
	}
	else if (informations.size() == 4)
	{
		result = split(informations[0], ' ');
		if (result[0].compare("CAP") == 0 && result[1].compare("LS") == 0 && result.size() == 2)
		{
			// add prexi error : faire ca propre + recupérer les infos avant 
			std::vector<std::string> result;
			std::string msg("464 :Password incorrect\n");
			removeLines(informations[1]);
			std::cout << BOLD_YELLOW <<  informations[1] << RESET << std::endl;
			result = split(informations[1], ' ');
			if (result[1].compare("PASS") != 0)
			{
				send(this->socketFd, msg.c_str(), msg.size(), 0);
				this->server.terminate();
				exit(EXIT_FAILURE);
			}
			//std::cout << BOLD_RED << "Error, cannot authentitate user\n";
		}
	}
	else
		std::cout << "BUFFER NOT FULL\n" << std::endl;
}
/*
	PREALABLE:
	
	vector<std::string>- > stock le split de la string user_infos avec \r

	IF (vector.size() == 3)

		STEP ONE : CAP LS 
		// split du vector[0] avec espaces -> on stocke dans un vector.
		// si vector.*begin() == CAP && vector.*begin++ == LS && vector.size() == 2
			-> CAP LS FLAG = OK -> fill data
			else
				error authentification 

		STEP TWO : NICK 
		// split du vector[0] avec espaces -> on stocke dans un vector.
		// si vector.begin() == NICK && vector.size() == 2
				-> NICK FLAG = OK -> fill data
		   else 
			error authentification 

		STEP THREE : USER 
		// split du vector[0] avec espace -> on stocke dans un vector.
		IF vector.begin() == USER && vector.size() == 5
			-> fill data : vector[1], vector[2], vector[3] = username, hostname, servername
			-> split du vector[4] avec : && stockage dans realName.

			FLAG INFOS USER RECUP => bool true
		ELSE
			error authentification 
	
	ELSE : on continue de recevoir
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
