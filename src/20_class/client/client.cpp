#include "client.hpp"

Client::Client(Server& serv):
server(serv), bufferSize(BUFFER_SIZE), status(WAITING),hasNick(false)
{ 
	this->userInfos.invisibleMode = true;
	this->userInfos.operatorMode = false;
	this->lastPingSent = time(0);
	this->lastPongReceived = time(0);
	
}

Client::~Client(){ }

std::string	Client::getPrefix( void )
{
	return(":" + this->userInfos.nickName + "!" + this->userInfos.userName + "@" + this->userInfos.hostName); // add espace quand error.hpp added
}

void	Client::deconnectClient( void )
{
	//std::cout << BOLD_RED << "DISCONNECTING CLIENT" << RESET << std::endl;
	for (std::vector<pollfd>::iterator it = this->server.fdListened.begin(); it != this->server.fdListened.end(); it++)
	{
		if (this->socketFd == it->fd)
			it->revents = -1; 
	}
}


// TO KEEP UNTIL WE FINISH 
// ANCIENNE BOUCLE D'envoi de la commande // JULIA
/* 

std::string		removeLines( std::string toSplit )
{
	for (size_t i = 0; i < toSplit.size(); i++) {
        if (toSplit[i] == '\n') {
            toSplit.erase(i, 1);
        }
    }
	return(toSplit);
}

void	Client::ping()
{
	sendMessage(PING(this->userInfos.hostName));
}


// TO KEEP UNTIL WE FINISH 

// ANCIENNE BOUCLE D'envoi de la commande // JULIA
/* 
	memset(client.buffer, 0, client.bufferSize);
	client.authentificationCmd.clear();
	std::vector<std::string> tmp;
	
	recv(client.socketFd, client.buffer, client.bufferSize - 1, 0);
	std::cout << BOLD_YELLOW << "buffer read: -->" << YELLOW << client.buffer << BOLD_YELLOW << "<--" << RESET << std::endl;
	client.cmd += client.buffer;
	//std::cout << "client.cmd:" << client.cmd << std::endl;
	if (client.cmd.find("\r"))
	{
		//std::cout << "here split" << std::endl;
		tmp = split(client.cmd, '\r');
		for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end() ; it++)
		{
			std::string tmp = *it;
			tmp = removeLines(tmp);
			if (!tmp.empty())
			{
				//std::cout << YELLOW << "|" << tmp << "|" << RESET << std::endl;
				client.authentificationCmd.push_back(tmp);
			}	
		}
	}
	else
	{
		//std::cout << "here" << std::endl;
		tmp = split(client.authentification, "\n");
		client.authentificationCmd.push_back(tmp[0]);
	}
	if (client.authentificationCmd.size() > 1)
		client.isIrssi = true;
	for (std::vector<std::string>::iterator it = client.authentificationCmd.begin(); it != client.authentificationCmd.end(); it++)
	{
		std::cout << BOLD_YELLOW << "launchCommand: -->" << YELLOW << *it << BOLD_YELLOW << "<--" << RESET << std::endl;
		client.launchCommand(*it);
	}
	client.cmd.clear();
*/