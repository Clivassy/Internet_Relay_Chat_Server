#include "client.hpp"

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
			//std::cout << BOLD_WHITE << "Client Mode has been updated to : INVISIBLE" << std::endl;
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
			//std::cout << BOLD_WHITE << "Client Mode has been updated to : NOT INVISIBLE" << std::endl;
		}
		if (argMode == 'o')
		{
			this->userInfos.invisibleMode = false;
			sendMessage(getPrefix() + " 221 " + this->userInfos.nickName + " " + "-" + character + "\r\n");
			//std::cout << BOLD_WHITE << "Client Mode has been updated to : NOT OPERATOR" << std::endl;
		}
		else if (argMode != 'i' and argMode != 'o')
		{
			sendMessage(getPrefix() + " 472 " + character + " :is unknown mode char to me\r\n");
		}
	}
	return (1);
}

bool	Client::cmdMODE(std::vector<std::string> &cmd)
{
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
	std::cout << BOLD_RED << "SIZE = " << cmd.size() << RESET << std::endl;
	if (cmd.size() <= 2)
	{
		sendMessage(ERR_NEEDMOREPARAMS("MODE"));
		return (false);
	}
	if (cmd[1].compare(this->userInfos.nickName) != 0)
	{
		sendMessage(ERR_USERSDONTMATCH(this->userInfos.nickName));
		return (false);
	}
	// /!\ gérer si on trouve aucun + ou - dans l'input ? 
	if (cmd.size() == 3) // CAS ou un mode ou plusieurs modes sont collés : `MODE nickname +i-o`
	{
		parseModes(cmd[2]);
		return(true);
	}
	else if (cmd.size() > 3) // CAS ou un mode ou plusieurs modes sont séparé en différents arg  : `MODE nickname +i -o`
	{
		std::cout << BOLD_GREEN << "Argument plusieurs  mots" << cmd[0] << RESET << std::endl;
		for (std::vector<std::string>::iterator it = cmd.begin() ; it != cmd.end(); it++)
			parseModes(*it);
		return (true);
	}
	return (false);
	
	//----------------------------------------
	// CODE ARZU : 
	/*if (cmd.size() < 2)
	{
		sendMessage(ERR_NEEDMOREPARAMS("MODE"));
		return (false);
	}
	return (true);
	-------------------------------------------------*/
}