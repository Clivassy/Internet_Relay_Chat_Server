#include "client.hpp"

Client::Client(Server& serv):
server(serv), bufferSize(BUFFER_SIZE), isAuthentified(false), isConnected(false), isOperator(false),
isValidPassword(false), status(COMING)
{ }

Client::~Client(){ }

std::string	Client::getPrefix( void )
{
	return(":" + this->userInfos.nickName + "!" + this->userInfos.userName + "@" + this->userInfos.hostName); // add espace quand error.hpp added
}

void	Client::errorAuthentification( void )
{
	std::cout << BOLD_RED << "Error: client cannot authenticate" << RESET << std::endl;
	this->server.terminate();
	exit(EXIT_FAILURE);
}

void	Client::errorPassword( void )
{
	std::string msg(this->getPrefix() + " 464 " + this->userInfos.nickName + " :Password incorrect\r\n");
	send(this->socketFd, msg.c_str(), msg.size(), 0);

	// recupérer le fd dans pollfd
	for (std::vector<pollfd>::iterator it = this->server.fdListened.begin(); it != this->server.fdListened.end(); it++)
	{
		if (this->socketFd == it->fd)
			it->revents = -1; 
	}
}

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

	if (send(this->socketFd, this->userInfos.userMessage.c_str(), this->userInfos.userMessage.size(), 0) < 0)
	{
		std::cout << BOLD_RED << "Error with send()" << std::endl;
		perror("send");
		this->server.terminate();
		exit(EXIT_FAILURE);
	}
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

std::string   Client::getPassword( std::string toSplit )
{
	std::vector<std::string> result;

	result = split(toSplit, ' ');
	if (result.size() > 1)
	{
		if ( result.size() == 2)
		{
			return (result[1]);
		}
	}
	return (NULL);
}

bool    Client::getNickName( std::string toSplit )
{
	std::vector<std::string> result;

	result = split(toSplit, ' ');
	if (result.size() == 2)
	{
		this->userInfos.nickName = result[1];
		return(true);
	}
	return (false);
}

bool    Client::getUserInfos( std::string toSplit )
{
	std::vector<std::string> result;
	std::vector<std::string> username;

	result = split(toSplit, ':');
	std::cout << result[0] << std::endl;
	std::cout << result[1] << std::endl;

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
			return(true);
		}
	}
	return (false);
}
/*
void    Client::fillDataUser( void )
{
	if (this->authentificationCmd.size() > 0)
	{
		if (this->authentificationCmd[0].compare("CAP LS") == 0)
		{
			if (this->Password == false)
			{
				if (authentificationCmd.size() > 1 )
				{
					std::string ret = getPassword(authentificationCmd[1]);
					this->Password = true;
					if (ret.compare(this->server.password) != 0)
						this->isValidPassword = false;
				}
			}
			if ( this->NICK2 == false)
			{
				if (authentificationCmd.size() > 2 )
				{
					if (this->getNickName(this->authentificationCmd[2]) == true)
					{
						std::cout << this->userInfos.nickName << std::endl;
						this->NICK2 = true;
					}
					else
					{
						if (this->isValidPassword == false)
							this->errorPassword();
					}
				}
			}
			if ( this->USER == false)
			{
				if (this->authentificationCmd.size() > 3) 
				{
					this->getUserInfos(authentificationCmd[3]);
					this->USER = true;
				}
			}
			if (this->USER)
			{
				if (this->isValidPassword == false)
					this->errorPassword();
				else
				{
					this->isAuthentified = true;
				}
			}

		}*/
		//if (authentificationCmd.size() == 2)
		//{
		//	std::cout << RED << "PASS : OK" << RESET << std::endl;
		//	std::cout  << GREEN << this->authentificationCmd[1] << RESET << std::endl;
//
		//	// check du PASS a faire ici
		//	// changer le flag ici 
		//}
		//if (authentificationCmd.size() == 3)
		//{
		//	std::cout << RED << "NICK2 : OK" << RESET << std::endl;
		//	// get nickname 
		//	if (this->getNickName(this->authentificationCmd[2]) == true)
		//		std::cout  << GREEN << this->userInfos.nickName << RESET << std::endl;
		//}
		//if(this->authentificationCmd.size() == 4)
		//{
		//	std::cout << RED << "USER: OK" << RESET << std::endl;
		//	// get userName
		//	std::cout  << GREEN << this->authentificationCmd[3] << RESET << std::endl;
		//}
	//}



	/*std::vector<std::string> informations;
	std::vector<std::string> result;

	informations = split(this->authentification, '\r');
	if (informations.size() == 5)
	{
		result = split(informations[0], ' ');
		if (result[0].compare("CAP") == 0 && result[1].compare("LS") == 0 && result.size() == 2)
		{
			std::string pass = this->getPassword(informations[1]);
			if (!pass.empty())
			{
				if (this->getNickName(informations[2]) == true)
				{
					if (this->getUserInfos(informations[3]) == true)
						isAuthentified = true;
					else
						this->errorAuthentification();
				}
				else 
					this->errorAuthentification();

				if (pass.compare(this->server.password) != 0)
					this->errorPassword();
			}
			else
				this->errorAuthentification();
			return;
		}					
	}
	else if (informations.size() == 4)
	{
		result = split(informations[0], ' ');
		if (result[0].compare("CAP") == 0 && result[1].compare("LS") == 0 && result.size() == 2)
		{
			std::vector<std::string> result;
			if (this->getNickName(informations[1]) == true)
			{
				if (this->getUserInfos(informations[2]) == true)
					this->errorPassword();
			}
		}
	}*/
//}
