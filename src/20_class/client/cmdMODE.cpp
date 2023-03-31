#include "client.hpp"
#define TRUE 1
#define FALSE 0

int		Client::parseModes(std::string modes)
{
	size_t j = 0;
	std::cout << BOLD_BLUE << "Argument en un seul mot" << modes << RESET << std::endl;
	for (j = 0; j < modes.size(); j++)
	{
		if (modes[j] == '+' or modes[j] == '-')
		{
			//std::cout << BOLD_YELLOW << "----> SEND to UPDAT MODE "<< RESET << std::endl;
			//std::cout << BOLD_YELLOW << "---->  " << cmd[2][j] << RESET << std::endl;
			//std::cout << BOLD_YELLOW << "---->  " << cmd[2][j + 1] << RESET << std::endl;
			updateMode(modes[j], modes[j + 1]);
		}
	}
	return(0);
}

int		Client::updateMode(char sign, char argMode)
{
	std::string character(1, argMode);
	if (sign == '+')
	{
		if (argMode == 'i')
		{
			this->userInfos.invisibleMode = true;
			sendMessage(getPrefix() + " 221 " + this->userInfos.nickName + " " + "+" + character + "\r\n");
		}
		else 
		{
			if (argMode !='o')
				sendMessage(getPrefix() + " 472 " + character + " :is unknown mode char to me\r\n");
		}
	}
	else 
	{	
		if (argMode == 'i')
		{			
			this->userInfos.invisibleMode = false;
			sendMessage(getPrefix() + " 221 " + this->userInfos.nickName + " " + "-" + character + "\r\n");
		}
		if (argMode == 'o')
		{
			this->userInfos.invisibleMode = false;
			sendMessage(getPrefix() + " 221 " + this->userInfos.nickName + " " + "-" + character + "\r\n");
		}
		else if (argMode != 'i' and argMode != 'o')
		{
			sendMessage(getPrefix() + " 472 " + character + " :is unknown mode char to me\r\n");
		}
	}
	return (1);
}

bool	Client::addUserMode(std::vector<std::string> cmd)
{
	if (cmd[1].compare(this->userInfos.nickName) != 0)
	{
		sendMessage(ERR_USERSDONTMATCH(this->userInfos.nickName));
		return (false);
	}
	if (cmd.size() == 3)
	{
		parseModes(cmd[2]);
		return(true);
	}
	else if (cmd.size() > 3)
	{
		for (std::vector<std::string>::iterator it = cmd.begin() ; it != cmd.end(); it++)
			parseModes(*it);
		return (true);
	}
	return (true);
}

bool	Client::addChannelMode(std::vector<std::string> cmd)
{
	(void)cmd;
	std::cout << BOLD_RED << "CHANNEL MODE  " << RESET << std::endl;
	if (this->userInfos.operatorMode == false)
		sendMessage(ERR_CHANOPRIVSNEEDED(this->server.getChannel(this->userInfos.nickName)->first));
	/*
	ERRORS : 
	- ERR_CHANOPRIVNEEDED : user need to be operator
	- ERR_NOSUCHNICK : user does not exist in server
	Faire une fct pour checker si un client est present dans le server. 
	-> RECUPERER LE CHAN : 
	- ERR_NOSUCHCHANNEL : channel does not exit 
	isChannelExisting(std::string name);
	- ERR_NOTONCHANNEL : user is not in the channel
	isClientExisting(std::string name);

	ELSE 
		-> PARSER LE MODE : UNIQUEMENT +I OU -I ACCEPTED
		-> TURN BOLEEAN TRUE OR FALSE. 
		-> SENDMESSAGE TO USER WHO CHANGED MODE : channel name + new mode. 
	*/
	return (true);
}

bool	Client::cmdMODE(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return(false);
	if (cmd.size() < 2)
	{
		sendMessage(ERR_NEEDMOREPARAMS("MODE"));
		return (false);
	}
	if (cmd[1].find("#") != std::string::npos)
    	this->addChannelMode(cmd);
	else
		this->addUserMode(cmd);
	return (true);
}
	// cmd : MODE <target> <mode changes>
	// ex : MODE nickname +i-o+
	// NB : unlimited number of mode changes in the same command.
	// differents can be separated by space OR sticked togheter.

	// ERR_NEEDMOREPARAMS              
	// ERR_USERSDONTMATCH
    // ERR_UMODEUNKNOWNFLAG :: a la mano car pas dans le define
	// RPL_UMODEIS :: a la mano car pas dans le define
	
	// 1) Parsing the Command 
	// 2) Validating the Command
	// 3) Updating Modes