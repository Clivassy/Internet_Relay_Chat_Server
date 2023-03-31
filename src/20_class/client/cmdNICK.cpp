#include "client.hpp"

// The NICK message may be sent from the server to clients to acknowledge their NICK command was successful,  // ARZU
// and to inform other clients about the change of nickname. // ARZU
// In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.}// ARZU
void	Client::checkDoubleNICK(std::string cmd, std::string errMsg)
{
	for (std::vector<Client>::iterator it = this->server.clientList.begin(); it != this->server.clientList.end(); it++ )
	{
		if ((*it).userInfos.nickName.compare(cmd) == 0)
		{
			sendMessage(errMsg);
			this->deconnectClient();
			return ;
		}
	} 
}

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
		if (this->status == REGISTERED)
		{
			checkDoubleNICK(cmd[1], ERR_NICKNAMEINUSE(cmd[1])); 
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
		//std::cout << BOLD_PURPLE << "NICKNAME CHANGE" << RESET << std::endl;
		//sendMessage(NICK_INFORM(this->userInfos.nickName,this->userInfos.userName,this->userInfos.hostName,cmd[1]));
		checkDoubleNICK(cmd[1], ERR_NICKNAMEINUSE(cmd[1])); // change for ERR_NICKCOLLISION? 
		sendMessage(NICK(this->userInfos.nickName, cmd[1]));
		this->userInfos.nickName = cmd[1];
		return (true);
	}
	return (false);
}