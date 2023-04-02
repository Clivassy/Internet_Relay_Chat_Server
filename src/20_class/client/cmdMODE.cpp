#include "client.hpp"

#define USER_MODE 0
#define CHANNEL_MODE 1

int		Client::parseModes(std::string modes, int modeType, std::string user)
{
	size_t j = 0;
	for (j = 0; j < modes.size(); j++)
	{
		if (modes[j] == '+' or modes[j] == '-')
		{
			if (modeType == USER_MODE)
				updateUserModes(modes[j], modes[j + 1]);
			else 
				updateChannelModes(modes[j], modes[j + 1], user);
		}		
	}
	return(0);
}

int		Client::updateChannelModes(char sign, char mode, std::string user)
{
	switch (sign)
	{
	case '+':
		if (mode == 'i')
		{
			this->server.getChannel(this->userInfos.nickName)->second.isInviteOnly = true;
			sendMessage(this->getPrefix() + RPL_CHANNELMODEIS(this->userInfos.nickName, "+i", user));
		}
		else if (mode == 'o')
		{
			// pour l'instant la liste est vide et non initialisée
			if (this->server.getChannel(this->userInfos.nickName)->second.clientOperators.size() > 0)
				this->server.getChannel(this->userInfos.nickName)->second.clientOperators.insert(user);
			sendMessage(this->getPrefix() + RPL_CHANNELMODEIS(this->userInfos.nickName, "+o", user));
		}
		else
			sendMessage(getPrefix() + " 472 " + mode + " :is unknown mode char to me\r\n");
		break;
	case '-':
		if (mode == 'i')
		{
			this->server.getChannel(this->userInfos.nickName)->second.isInviteOnly = false;
			sendMessage(this->getPrefix() + RPL_CHANNELMODEIS(this->userInfos.nickName, "-i", user));
		}
		else if (mode == 'o')
		{
			// pour l'instant la liste est vide et non initialisée
			if (this->server.getChannel(this->userInfos.nickName)->second.clientOperators.size() > 0)
				this->server.getChannel(this->userInfos.nickName)->second.clientOperators.erase(user);
			sendMessage(this->getPrefix() + RPL_CHANNELMODEIS(this->userInfos.nickName, "-o", user));
		}
		else
			sendMessage(getPrefix() + " 472 " + mode + " :is unknown mode char to me\r\n");
	default:
		std::cout << BOLD_RED << "ERROR" << RESET << std::endl;
		break;
	}
	return (0);
}

int		Client::updateUserModes(char sign, char mode)
{
	std::string character(1, mode);
	if (sign == '+')
	{
		if (mode == 'i')
		{
			this->userInfos.invisibleMode = true;
			sendMessage(getPrefix() + RPL_UMODEIS(this->userInfos.nickName, "+", character));
		}
		else 
		{
			if (mode !='o')
				sendMessage(getPrefix() + " 472 " + character + " :is unknown mode char to me\r\n");
		}
	}
	else 
	{	
		if (mode == 'i')
		{			
			this->userInfos.invisibleMode = false;
			sendMessage(getPrefix() + RPL_UMODEIS(this->userInfos.nickName, "-", character));
		}
		if (mode == 'o')
		{
			this->userInfos.invisibleMode = false;
			sendMessage(getPrefix() + RPL_UMODEIS(this->userInfos.nickName, "-", character));
		}
		else if (mode != 'i' and mode != 'o')
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
		parseModes(cmd[2], USER_MODE, "");
		return(true);
	}
	else if (cmd.size() > 3)
	{
		for (std::vector<std::string>::iterator it = cmd.begin() ; it != cmd.end(); it++)
			parseModes(*it, USER_MODE, "");
		return (true);
	}
	return (true);
}

// CASE 1) Not enough parameter.
// CASE 2) Channel does not exist.
// CASE 3) Client asking is not on the channel. 
// CASE 4) Client asking is not channel operator. 
// CASE 5) If +o/-o, User we want to change mode is not on the channel.
bool	Client::parsingErrorChannel(std::vector<std::string> cmd)
{
	std::string channelName = "";
	if (cmd.size() < 3)
	{
		sendMessage(ERR_NEEDMOREPARAMS("MODE"));
		return (false);
	}
	channelName = cmd[1];
	toLowerStr(channelName);
	if (!this->server.isChannelExisting(channelName))
	{
		sendMessage(ERR_NOSUCHCHANNEL(channelName));
		return (false);
	}
	if (!this->server.getChannel(channelName)->second.isclientConnected(this->userInfos.nickName))
	{
		sendMessage(ERR_NOTONCHANNEL(channelName));
		return (false);
	}
	// checker si le client qui fait la commande est channel operator
	if (!this->server.getChannel(channelName)->second.isClientOperatorChannel(this->userInfos.nickName)) 
		sendMessage(ERR_CHANOPRIVSNEEDED(channelName));
	if (cmd.size() == 4)
	{
		if (!this->server.getChannel(channelName)->second.isclientConnected(cmd[3]))
		{
			sendMessage(ERR_NOSUCHNICK(cmd[3]));
			return (false);
		}
	}
	return (true);
}

bool	Client::addChannelMode(std::vector<std::string> cmd)
{
	if (!parsingErrorChannel(cmd))
		return(false);
	std::string targetName = "";
	if (cmd.size() == 4)
		targetName = cmd[3];
	parseModes(cmd[2], CHANNEL_MODE, targetName);
	return(true);
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
//------------------------------------------------------------------------
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
	// 3) Updating Modesmake
	