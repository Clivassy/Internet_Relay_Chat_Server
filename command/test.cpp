# include "test.hpp"
# include "../src/20_class/client.hpp"
#include <sys/types.h>
#include <sys/socket.h>

//source https://modern.ircdocs.horse/#client-messages

// The PASS command is used to set a ‘connection password’. If set, the password must be set before any attempt to register the connection is made. This requires that clients send a PASS command before sending the NICK / USER combination.
//	send(this->socketFd, this->userInfos.userMessage.c_str(), this->userInfos.userMessage.size(), 0);

void	Client::sendMessage(std::string str)
{
	send(this->socketFd, str.c_str(), str.size(), 0);
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
	if ((pos = this->server.get_password().find(" ")) == std::string::npos)
	{
		if (cmd[1] != this->server.get_password())
		{
			sendMessage(ERR_PASSWDMISMATCH);
			return (false);
		}
	}
	else
	{
		for (int i = 1; i < cmd.size(); i++)
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

// The NICK command is used to give the client a nickname or change the previous one.
bool	Client::cmdNICK(std::vector<std::string> &cmd, Client &clt)
{
	if (cmd[1].empty())
	{
		sendMessage(ERR_NONICKNAMEGIVEN);
		return (false);
	}
	if (cmd.size > 2)
	{
		sendMessage(ERR_ERRONEUSNICKNAME) 
		return (false);
	}
	for (int i = 0; i < srv.clientList.size(); i++)
	{
		if (cmd[1] == srv.clientList[i].ClientNick)
		{
			sendMessage(ERR_NICKNAMEINUSE);
			return (false);
		}
	}
	sendMessage(NICK(this->User.nickname, cmd[1]));
	this->User.nickname = cmd[1];


	//The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.

	return (true);
}
/*
// The USER command is used at the beginning of a connection to specify the username and realname of a new user.
bool	USER(std::vector<std::string> &cmd)
{
	std::cout << "USER" << std::endl;
	if (cmd[1].empty())
	{
		std::cout << ERR_NEEDMOREPARAMS("USER");
		return (false);
	}
	//if (cmd[1].length() < 1 || cmd[1].length() > 20) //20 (USERLEN cmd)
	// A CONTINUER
	return (true);
}

//The PING command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer.
bool	PING(std::vector<std::string> &cmd)
{
	std::cout << "PING" << std::endl;
	if (cmd[1].empty())
	{
		std::cout << ERR_NEEDMOREPARAMS("PING");
		return (false);
	}
	std::cout << "PONG" << cmd[1] << "\r" << std::endl;
	return (true);
}

//The PONG command is used as a reply to PING commands, by both clients and servers. The <token> should be the same as the one in the PING message that triggered this PONG.
bool	PONG(std::vector<std::string> &cmd)
{
	std::cout << "PONG" << std::endl;
	return (true);
}

// The OPER command is used by a normal user to obtain IRC operator privileges. Both parameters are required for the command to be successful.
bool	OPER(std::vector<std::string> &cmd, std::string &pw)
{
	std::cout << "OPER" << std::endl;
	if (cmd[2].empty())
	{
		std::cout << ERR_NEEDMOREPARAMS("OPER");
		return (false);
	}
	if (cmd[2] != pw)
	{
		std::cout << ERR_PASSWDMISMATCH;
		return (false);
	}
	// If the supplied name and password are both correct, and the user is connecting from a valid host, the RPL_YOUREOPER message is sent to the user. The user will also receive a MODE message indicating their new user modes, and other messages may be sent.
	std::cout << " :You are now an IRC operator" << std::endl;
	return (true);
}

// The QUIT command is used to terminate a client’s connection to the server. The server acknowledges this by replying with an ERROR message and closing the connection to the client.
bool	QUIT(std::vector<std::string> &cmd)
{
	std::cout << ":QUIT : Bye for now" << std::endl;
	return (true);
}

// This message is sent from a server to a client to report a fatal error, before terminating the client’s connection.
bool	ERROR(std::vector<std::string> &cmd)
{
	std::cout << "ERROR" << std::endl;
	return (true);
}

// The JOIN command indicates that the client wants to join the given channel(s), each channel using the given key for it. The server receiving the command checks whether or not the client can join the given channel, and processes the request. Servers MUST process the parameters of this command as lists on incoming commands from clients, with the first <key> being used for the first <channel>, the second <key> being used for the second <channel>, etc.
bool	JOIN(std::vector<std::string> &cmd)
{
	std::cout << "JOIN" << std::endl;
	if (cmd[1].empty())
	{
		std::cout << ERR_NEEDMOREPARAMS("JOIN");
		return (false);
	}
	// Les erreurs possible
	// ERR_TOOMANYCHANNELS
	if (cmd[1][0] != '#')
	{
		std::cout << ERR_BADCHANMASK(cmd[1]);
		return (false);
	}

	return (true);
}

// The PART command removes the client from the given channel(s). On sending a successful PART command, the user will receive a PART message from the server for each channel they have been removed from. <reason> is the reason that the client has left the channel(s).
bool	PART(std::vector<std::string> &cmd)
{
	std::cout << "PART" << std::endl;
	if (cmd[1].empty())
	{
		std::cout << ERR_NEEDMOREPARAMS("PART");
		return (false);
	}
	// ERR_NOSUCHCHANNEL if the channel doesn't exist
	// ERR_NOTONCHANNEL if you are not on the chanel
	return (true);
}

// The PRIVMSG command is used to send private messages between users, as well as to send messages to channels. <target> is the nickname of a client or the name of a channel.
bool	PRIVMSG(std::vector<std::string> &cmd)
{
	std::cout << "PRIVMSG" << std::endl;
	if (cmd[2].empty())
	{
		std::cout << ERR_NEEDMOREPARAMS("PRIVMSG");
		return (false);
	}
	return (true);
}
*/

// FAIRE UN sent a la place de std::cout 
//	send(this->socketFd, this->userInfos.userMessage.c_str(), this->userInfos.userMessage.size(), 0);
//bool	launchCommand::check_command()
bool	Client::launchCommand(void)
{
	std::vector<std::string> vecmd = split(this->cmd, ' ');

//	std::string	choice[9] = {"NICK", "USER", "PING", "PONG", "QUIT", "JOIN", "PART", "PRIVMSG", "ERROR"};
//	bool	(*f[9])(std::vector<std::string> &) = {&NICK, &USER, &PING, &PONG, &QUIT, &JOIN, &PART, &PRIVMSG, &ERROR};
	int i = 0;

	if (vecmd.empty())
		return (false);
	if (vecmd[0] == "PASS")
		return (cmdPASS(vecmd));
/*	if (this->isConnected && cmd[0] == "OPER")
		return (OPER(cmd, pw));
	while (this->isConnected && i < 11)
	{
		if (cmd[0] == choice[i])
			return (*f[i])(cmd);
		i++;
	}*/
	std::cout << "Commande not found" << std::endl;
	return (false);
}
