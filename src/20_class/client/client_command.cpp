# include "client.hpp"
# include <sys/types.h>
# include <sys/socket.h>
# include <vector>
# include <iostream>
# include <fstream>
# include <string>
# include "../../10_tools/errors.hpp"


//source https://modern.ircdocs.horse/#client-messages

void	Client::sendMessage(std::string str)
{
	send(this->socketFd, str.c_str(), str.size(), 0);
}

void	Client::sendOtherClient(std::string str)
{
	for (std::vector<Client>::iterator it = this->server.clientList.begin(); it != this->server.clientList.end(); it++)
	{
			send(it->socketFd, str.c_str(), str.size(), 0);
	}
}

bool	Client::cmdPASS(std::vector<std::string> &cmd)
{

	if (cmd.empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("PASS"));
		return (false);
	}
	/*if (cmd.size() == 1 or cmd.size() == 0 )
	{
		std::cout << YELLOW << "here" << RESET << std::endl;
		sendMessage(ERR_NEEDMOREPARAMS("PASS"));
		return (false);
	}
	if (cmd.size() == 2)
	{
		if (this->status == REGISTERED)
		{
			sendMessage(ERR_ALREADYREGISTERED);
			return (true);
		}
		else if (this->status == COMING)
		{
			if (this->server.get_password().compare(cmd[1]) == 0)
			{
				std::cout << "PASSWORD IS OK" << std::endl;
				this->status = REGISTERED;
				return (true);
			}
			else
			{
				this->status = COMING;
				sendMessage(ERR_PASSWDMISMATCH);
				return(false);
			}
		}
	}*/
	return (true);
}

// The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, 
// and to inform other clients about the change of nickname. 
// In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.}
bool	Client::cmdNICK(std::vector<std::string> &cmd)
{
	if (cmd.size() == 1)
	{
		sendMessage(ERR_NONICKNAMEGIVEN);
		return (false);
	}
	/*if (cmd.size() > 2)
	{
		sendMessage(ERR_ERRONEUSNICKNAME(cmd[1])); 
		return (false);
	}
	if (cmd.size() == 2)
	{
		if (this->status == REGISTERED)
		{
			this->userInfos.nickName = cmd[1];
			return (true);
		}
		if (this->status == CONNECTED)
		{
			this->userInfos.nickName = cmd[1];
			sendMessage(NICK(this->userInfos.nickName, cmd[1]));
			sendMessage(RPL_WELCOME(this->userInfos.nickName, this->userInfos.userName ,this->userInfos.hostName ));
			return (true);
		}
	}*/
	return (true);
}


bool	Client::cmdUSER(std::vector<std::string> &cmd)
{
	(void)cmd;
	//------- Pré authentification pour Yann et Arzu (TEMPORAIRE) ----// 

	if (this->status == REGISTERED)
	{
		this->userInfos.nickName = "jbatoro";
		this->userInfos.userName = "jbatoro";
		this->userInfos.hostName = "jbatoro";
		this->userInfos.realName = "Julia Batoro"; 
		sendMessage(RPL_WELCOME(this->userInfos.nickName, this->userInfos.userName ,this->userInfos.hostName));
		this->status = CONNECTED;
	} 
	//------------------------------------------------------------ //
	/*std::vector<std::string> user;
	
	std::cout << BOLD_PURPLE << cmd[0] << RESET << std::endl;
	if (cmd.size() == 1)
	{
		std::cout << BOLD_PURPLE << "CAS a gerer dans NC =>  " << cmd[0] << RESET << std::endl;
	}
	if (cmd.size() == 2) //cas de irssi ou c'est separé par deux points
	{
		std::cout << BOLD_PURPLE << "CAS a gerer dans IRSSI =>  " <<  cmd[0] << " | " << cmd[1] << RESET << std::endl;
	}*/
	
	/*std::cout << BOLD_RED << cmd.size() << std::endl;
	if (this->status == REGISTERED and !cmd.empty())
	{
		user = split(cmd[0], ' ');

		if (cmd.size() == 1)
		{
			if (user.size() != 5)
			{
				sendMessage(ERR_NEEDMOREPARAMS("USER"));
				return (false);
			}
			this->userInfos.realName = user[3];
		}
		else if (cmd.size() == 2)
		{
			if (cmd[1].empty())
			{
				sendMessage(ERR_NEEDMOREPARAMS("USER"));
				return (false);
			}
			this->userInfos.realName = cmd[1];
		}
		this->userInfos.userName = user[1];
		this->userInfos.hostName = user[2];
		sendMessage(RPL_WELCOME(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName));
		this->status = CONNECTED;
	}
	else if (this->status == CONNECTED)
	{
		sendMessage(ERR_ALREADYREGISTERED);
		return(false);
	}*/

	return (true);
}

bool	Client::cmdPING(std::vector<std::string> &cmd)
{
	if (this->status == CONNECTED)
	{
		if (cmd[1].empty())
		{
			sendMessage(ERR_NEEDMOREPARAMS("PING"));
			return (false);
		}
		std::cout << "PONG " + cmd[1] + "\r" << std::endl;
		sendMessage("PONG " + cmd[1] + "\r");
	}

	return (true);
}

bool	Client::cmdOPER(std::vector<std::string> &cmd)
{
	if (cmd[1].empty() || cmd[2].empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("OPER"));
		return (false);
	}
	if (cmd[2] != this->server.get_password())
	{
		sendMessage(ERR_PASSWDMISMATCH);
		return (false);
	}
	sendMessage(RPL_YOUREOPER(cmd[1]));
	//The user will also receive a MODE message indicating their new user modes, and other messages may be sent.
	return (true);
}

bool	Client::cmdQUIT(std::vector<std::string> &cmd)
{
	// This is typically only dispatched to clients that share a channel with the exiting user.
	// Envoye Quit :<reason> au autres clients
	(void) cmd;
	sendMessage("QUIT : Bye for now");
	return (true);
}


// Supprime un client ou des channels
bool	Client::cmdPART(std::vector<std::string> &cmd)
{
	if (cmd[1].empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("PART"));
		return (false);
	}
	// ERR_NOSUCHCHANNEL si le channel n'exidte pas 
	// ERR_NOTONCHANNEL si le channel existe mais le client n'est pas dedans
	
	// the user will receive a PART message from the server for each channel they have been removed from. <reason> is the reason that the client has left the channel(s).
	// Servers SHOULD NOT send multiple channels in this message to clients, and SHOULD distribute these multiple-channel JOIN messages as a series of messages with a single channel name on each.
	return (true);
}

// The PRIVMSG command is used to send private messages between users, as well as to send messages to channels. <target> is the nickname of a client or the name of a channel.
bool	Client::cmdPRIVMSG(std::vector<std::string> &cmd)
{
	if (cmd[2].empty())
	{
		std::cout << ERR_NEEDMOREPARAMS("PRIVMSG");
		return (false);
	}
	//If <target> is a channel name and the client is banned and not covered by a ban exception, the message will not be delivered and the command will silently fail.
	// Si le message ne peut pas etre envoye ERR_CANNOTSENDTOCHAN
	// If <target> is a channel name, it may be prefixed with one or more channel membership prefix character (@, +, etc) and the message will be delivered only to the members of that channel with the given or higher status in the channel. Servers that support this feature will list the prefixes which this is supported for in the STATUSMSG RPL_ISUPPORT parameter, and this SHOULD NOT be attempted by clients unless the prefix has been advertised in this token.
	// If <target> is a user and that user has been set as away, the server may reply with an RPL_AWAY (301) numeric and the command will continue.

	// When the PRIVMSG message is sent from a server to a client and <target> starts with a dollar character ('$', 0x24), the message is a broadcast sent to all clients on one or multiple servers.
	return (true);
}

bool	Client::cmdNOTICE(std::vector<std::string> &cmd)
{
	//The difference between NOTICE and PRIVMSG is that automatic replies must never be sent in response to a NOTICE message.This rule also applies to servers – they must not send any error back to the client on receipt of a NOTICE command.
	// SI j'ai bien compris un NOTICE est un PRIVMSG sans message d'erreur
	if (cmd[2].empty())
		return (false);
	// Envoyer le message
	return (true);
}

////////////////////////////////////////////////////////////////////////////
bool	Client::cmdMODE(std::vector<std::string> &cmd)
{
	if (cmd[1].empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("MODE"));
		return (false);
	}
	return (true);
}

// Parameters: <nickname> <channel>
bool	Client::cmdINVITE(std::vector<std::string> &cmd)
{
	if (cmd[2].empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("INVITE"));
		return (false);
	}
	// Si le channel n'existe pas => ERR_NOSUCHCHANNEL
	// Si le client n'est pas dans le channel dont il a envoye l'invitation => ERR_NOTONCHANNEL
	// Si le client est deja dans le channel => ERR_USERONCHANNEL
	
	//When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.
	return (true);
}

//  Parameters: <channel> <user> [<comment>]
bool	Client::cmdKICK(std::vector<std::string> &cmd)
{
	if (cmd[2].empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("KICK"));
		return (false);
	}
	// supprime le user du channel
	return (true);
}

////////////////////////////////////////////////////////////////////////////
bool	Client::cmdWHOIS(std::vector<std::string> &cmd)
{
	if (cmd[1].empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("WHOIS"));
		return (false);
	}
	return (true);
}

bool	Client::cmdCAP(std::vector<std::string> &cmd)
{
	if (cmd.size() == 2)
	{
		std::cout << RED << cmd[1] << std::endl;
		if (cmd[1].compare("LS") == 0)
		{
			this->status = COMING;
			return true;
		}
	}
	return (false);
}

bool	Client::cmdJOIN(std::vector<std::string> &cmd)
{
	(void)cmd;
	return true;
}

//- PASS, NICK, USER, PING, OPER, QUIT, JOIN, PART, PRIVMSG, NOTICE, MODE, INVITE. KICK, WHOIS
bool	Client::launchCommand(std::string command)
{
	std::vector<std::string> ccmd = split(command, ':');
	std::vector<std::string> vecmd = split(ccmd[0], ' ');
	std::string	choice[15] = {"CAP", "PASS", "NICK", "USER", "PING", "OPER", "QUIT", "JOIN", "PART", "PRIVMSG", "NOTICE", "MODE", "INVITE", "KICK", "WHOIS"};
	bool	(Client::*f[15])(std::vector<std::string> &) = {&Client::cmdCAP, &Client::cmdPASS, &Client::cmdNICK, &Client::cmdUSER, &Client::cmdPING, &Client::cmdOPER, &Client::cmdQUIT, &Client::cmdJOIN, &Client::cmdPART, &Client::cmdPRIVMSG, &Client::cmdNOTICE, &Client::cmdMODE, &Client::cmdINVITE, &Client::cmdKICK, &Client::cmdWHOIS};
	int i = 0;

	if (vecmd.empty())
	{
		return (false);
	}
	std::cout << RED << "STATUS :" << this->status << std::endl;
	
	//------- Pré authentification pour Yann et Arzu (TEMPORAIRE) ----// 
	if (this->status == COMING)
		this->status = REGISTERED;
	//------------------------------------------------------------ //
	while (i < 14)
	{
		if (vecmd[0].compare("CAP") == 0)
		{
			return (this->cmdCAP(vecmd));
		}
		if (vecmd[0].compare("PASS") == 0)
		{
			return (this->cmdPASS(vecmd));
		}
		if (vecmd[0].compare("USER") == 0)
		{
			std::cout << BOLD_CYAN << "Command called -----> " << choice[i] <<  std::endl;
			return(this->cmdUSER(ccmd));
		}
		if (vecmd[0] == choice[i])
		{
			std::cout << BOLD_CYAN << "Command called -----> " << choice[i] <<  std::endl;
			return (this->*f[i])(vecmd);
		}
		i++;
	}
	// sendMessage("Command not found");
	return (false);
}
