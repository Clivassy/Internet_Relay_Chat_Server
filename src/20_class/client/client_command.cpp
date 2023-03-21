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
	int pos = 0;
	int tmp_pos = 0;

	if (cmd[1].empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("PASS"));
		return (false);
	}
	// Check avec espace
	if ((pos = this->server.get_password().find(" ")) == (int)std::string::npos)
	{
		if (cmd[1] != this->server.get_password())
		{
			sendMessage(ERR_PASSWDMISMATCH);
			return (false);
		}
	}
	else
	{
		for (int i = 1; i < (int)cmd.size(); i++)
		{
			if (cmd[i] != this->server.get_password().substr(tmp_pos, pos))
			{
				sendMessage(ERR_PASSWDMISMATCH);
				return (false);
			}
			tmp_pos = pos + 1;
		}
	}
	this->isConnected = true;
	sendMessage("Connection established \r\n");
	return (true);
}

bool	Client::cmdNICK(std::vector<std::string> &cmd)
{
	if (cmd[1].empty())
	{
		sendMessage(ERR_NONICKNAMEGIVEN);
		return (false);
	}
	if (cmd.size() > 2)
	{
		sendMessage(ERR_ERRONEUSNICKNAME(cmd[1])); 
		return (false);
	}
	for (int i = 0; i < (int)this->server.clientList.size(); i++)
	{
		if (cmd[1] == this->server.clientList[i].userInfos.nickName)
		{
			sendMessage(ERR_NICKNAMEINUSE(cmd[1]));
			return (false);
		}
	}
	sendMessage(NICK(this->userInfos.nickName, cmd[1]));
	this->userInfos.nickName = cmd[1];
	//The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.
	return (true);
}

bool	Client::cmdUSER(std::vector<std::string> &cmd)
{
	// Deja enregistrer
	if (cmd[1].empty() || cmd[2].empty() || cmd[3].empty() || cmd[4].empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("USER"));
		return (false);
	}
	if (cmd[2] != "0" || cmd[3] != "*")
	{
		sendMessage("USER :Parameters incorrect");
		return (false);
	}

	//If a client tries to send the USER command after they have already completed registration with the server, the ERR_ALREADYREGISTERED reply should be sent and the attempt should fail.

	//If the client sends a USER command after the server has successfully received a username using the Ident Protocol, the <username> parameter from this command should be ignored in favour of the one received from the identity server.

	this->userInfos.userName = cmd[4];
	this->userInfos.realName = cmd[4];
	//Clients SHOULD use the nickname as a fallback value for <username> and <realname> when they don’t have a meaningful value to use.
	return (true);
}

bool	Client::cmdPING(std::vector<std::string> &cmd)
{
	if (cmd[1].empty())
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

//- PASS, NICK, USER, PING, OPER, QUIT, JOIN, PART, PRIVMSG, NOTICE, MODE, INVITE. KICK, WHOIS
bool	Client::launchCommand(std::string command)
{
	std::cout << "HERE launchCommand => " << command << std::endl;
	
	std::vector<std::string> ccmd = split(command, ':');
	std::vector<std::string> vecmd = split(ccmd[0], ' ');
	std::string	choice[14] = {"PASS", "NICK", "USER", "PING", "OPER", "QUIT", "JOIN", "PART", "PRIVMSG", "NOTICE", "MODE", "INVITE", "KICK", "WHOIS"};
	bool	(Client::*f[14])(std::vector<std::string> &) = {&Client::cmdPASS, &Client::cmdNICK, &Client::cmdUSER, &Client::cmdPING, &Client::cmdOPER, &Client::cmdQUIT, &Client::cmdJOIN, &Client::cmdPART, &Client::cmdPRIVMSG, &Client::cmdNOTICE, &Client::cmdMODE, &Client::cmdINVITE, &Client::cmdKICK, &Client::cmdWHOIS};
	int i = 0;

//	if (!ccmd[1].empty())
//		vecmd.push_back(ccmd[1]);
	if (vecmd.empty())
		return (false);
	std::cout << "vecmd " << vecmd[0] << std::endl;
	while (i < 13)
	{
		if (vecmd[0] == choice[i])
		{
			std::cout << "bouhhhh" << std::endl;
			return (this->*f[i])(vecmd);
		}
		i++;
	}
	sendMessage("Command not found");
	return (false);
}
