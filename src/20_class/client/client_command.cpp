# include "client.hpp"
# include <sys/types.h>
# include <sys/socket.h>
# include <vector>
# include <iostream>
# include <fstream>
# include <string>
# include "../../10_tools/errors.hpp"


//source https://modern.ircdocs.horse/#client-messages

/////////////////////////////////////////////////////////
void	Client::sendMessage(std::string str)
{
	send(this->socketFd, str.c_str(), str.size(), 0);
}

void	Client::sendOtherClient(std::string str)
{
	for (std::vector<Client>::iterator it = this->server.clientList.begin(); it != this->server.clientList.end(); it++)
	{
		if (it->socketFd != this->socketFd)
			send(it->socketFd, str.c_str(), str.size(), 0);
	}
}
////////////////////////////////////////////////////////

bool	Client::cmdPASS(std::vector<std::string> &cmd)
{
	if (this->status == COMING)
	{
		if (cmd.empty() or cmd.size() == 1)
		{
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
			if (this->server.get_password().compare(cmd[1]) == 0)
			{
				//std::cout << "PASSWORD IS OK" << std::endl; //JULIA 
				this->status = REGISTERED;
				return (true);
			}
			else
			{
				//std::cout << BOLD_RED << "WRONG PASSWORD" << RESET << std::endl; //JULIA
				sendMessage(this->getPrefix() + " 464 " + this->userInfos.nickName + ERR_PASSWDMISMATCH);
				this->status = COMING;
				//this->deconnectClient(); // no need to deconnect? version Marie + William : a discuter //JULIA
				return(false);
			}
		}		
	}
	return (false);
}

// The NICK message may be sent from the server to clients to acknowledge their NICK command was successful,  // ARZU
// and to inform other clients about the change of nickname. // ARZU
// In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.}// ARZU
bool	Client::cmdNICK(std::vector<std::string> &cmd)
{
	if (this->status == REGISTERED or this->status == CONNECTED)
	{
		if (cmd.size() == 1)
		{
			sendMessage(ERR_NONICKNAMEGIVEN);
			return (false);
		}
		if (cmd.size() > 2)
		{
			sendMessage(ERR_ERRONEUSNICKNAME(cmd[1])); 
			return (false);
		}
	if (this->status == REGISTERED )
	{
		if (cmd.size() == 2)
		{

			this->userInfos.nickName = cmd[1];
			this->hasNick = true;
			return (true);
		}
	}
	}
	if (this->status == CONNECTED)
	{
		std::cout << BOLD_PURPLE << "NICKNAME CHANGE" << RESET << std::endl;
		// FIX : affichage x2 du changement de nickname // OU Pas d'affichage du tout 
		//sendMessage(NICK_INFORM(this->userInfos.nickName,this->userInfos.userName,this->userInfos.hostName,cmd[1]));
		sendMessage(NICK(this->userInfos.nickName, cmd[1]));
		this->userInfos.nickName = cmd[1];
		return (true);
	}
	return (false);
}


bool	Client::cmdUSER(std::vector<std::string> &cmd)
{
	//------- Pré authentification pour Yann et Arzu (TEMPORAIRE) ----// JULIA
	/*if (this->status == REGISTERED)
	{
		this->userInfos.nickName = "jbatoro";
		this->userInfos.userName = "jbatoro";
		this->userInfos.hostName = "jbatoro";
		this->userInfos.realName = "Julia Batoro"; 
		sendMessage(RPL_WELCOME(this->userInfos.nickName, this->userInfos.userName ,this->userInfos.hostName));
		this->status = CONNECTED;
	} */
	//------------------------------------------------------------ //
	
	/*std::cout << BOLD_PURPLE << cmd[0] << RESET << std::endl; // JULIA ::commentaire debeug
	if (cmd.size() == 1)
	{
		std::cout << BOLD_PURPLE << "CAS a gerer dans NC =>  " << cmd[0] << RESET << std::endl;
	}
	if (cmd.size() == 2) //cas de irssi ou c'est separé par deux points
	{
		std::cout << BOLD_PURPLE << "CAS a gerer dans IRSSI =>  " <<  cmd[0] << " | " << cmd[1] << RESET << std::endl;
	}*/
	if (this->status == REGISTERED && this->hasNick == true)
	{
		if (!cmd.empty())
		{
			if (cmd.size() != 5)
			{
				sendMessage(ERR_NEEDMOREPARAMS("USER"));
				return (false);
			}
			this->userInfos.userName = cmd[1];
			this->userInfos.hostName = cmd[2];
			this->userInfos.realName = cmd[4];
			sendMessage(RPL_WELCOME(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName));
			this->status = CONNECTED;
			return(true);
		}
		else if (this->status == CONNECTED)
		{
			sendMessage(ERR_ALREADYREGISTERED);
			return(false);
		}
	}
	return (false);
}

bool	Client::cmdPING(std::vector<std::string> &cmd)
{
	if (cmd.size() < 2)
	{
		sendMessage(ERR_NEEDMOREPARAMS("PING"));
		return (false);
	}
	std::cout << "PONG " + cmd[1] + "\r" << std::endl;
	sendMessage("PONG " + cmd[1] + "\r");
	return (true);
}

bool	Client::cmdOPER(std::vector<std::string> &cmd)
{
	if (cmd.size() < 3)
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
	if (cmd.size() == 1)
	{
		sendMessage(QUIT(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName));
		sendOtherClient(QUIT(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName));
	}
	else
	{
		sendMessage(QUIT_REASON(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, cmd[1]));	
		sendOtherClient(QUIT_REASON(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, cmd[1]));	
	}
	return (true);
}


// Supprime un client ou des channels
bool	Client::cmdPART(std::vector<std::string> &cmd)
{
	if (cmd.size() < 2)
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
	if (cmd.size() < 3)
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
	if (cmd.size() < 3)
		return (false);
	// Envoyer le message
	return (true);
}

////////////////////////////////////////////////////////////////////////////
bool	Client::cmdMODE(std::vector<std::string> &cmd)
{
	if (cmd.size() < 2)
	{
		sendMessage(ERR_NEEDMOREPARAMS("MODE"));
		return (false);
	}
	return (true);
}

// Parameters: <nickname> <channel>
bool	Client::cmdINVITE(std::vector<std::string> &cmd)
{
	if (cmd.size() < 3)
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
	if (cmd.size() < 3)
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
	if (cmd.size() < 2)
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
			return (true);
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
	if (command.empty())
		return (true);
	
	std::vector<std::string> ccmd = split(command, ':');
	std::vector<std::string> vecmd = split(ccmd[0], ' ');
	std::string	choice[15] = {"CAP", "PASS", "NICK", "USER", "PING", "OPER", "QUIT", "JOIN", "PART", "PRIVMSG", "NOTICE", "MODE", "INVITE", "KICK", "WHOIS"};
	bool	(Client::*f[15])(std::vector<std::string> &) = {&Client::cmdCAP, &Client::cmdPASS, &Client::cmdNICK, &Client::cmdUSER, &Client::cmdPING, &Client::cmdOPER, &Client::cmdQUIT, &Client::cmdJOIN, &Client::cmdPART, &Client::cmdPRIVMSG, &Client::cmdNOTICE, &Client::cmdMODE, &Client::cmdINVITE, &Client::cmdKICK, &Client::cmdWHOIS};
	int i = 0;

	if (ccmd.size() != 1 && ccmd.size() != 2)
	{
		sendMessage("Arguments incorrect : Too many ':'");
	}
	if (ccmd.size() == 2)
		vecmd.push_back(ccmd[1]);


	if (vecmd.empty())
	{
		return (false);
	}

	//------- Pré authentification pour Yann et Arzu (TEMPORAIRE) ----// JULIA::commentaire
	//	if (this->status == WAITING)
	//	this->status = REGISTERED;
	//	this->hasNick == true;
	////------------------------------------------------------------ //
	while (i < 14)
	{
		if (vecmd[0] == choice[i])
		{
			return (this->*f[i])(vecmd);
		}
		i++;
	}
	// sendMessage("Command not found"); //ARZU: commentaire
	return (false);
}
