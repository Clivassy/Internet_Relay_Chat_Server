#include "client.hpp"

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
			if (this->server.isClientExisting(cmd[1]))
			{
				sendMessage(this->getPrefix() + ERR_NICKNAMEINUSE(cmd[1]));
				this->online = false;
				return (false);
			}
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
		if (this->server.isClientExisting(cmd[1]))
		{
			sendMessage(this->getPrefix() + ERR_NICKNAMEINUSE(cmd[1]));
			return (false);
		}
		//sendMessage(NICK_INFORM(this->userInfos.nickName,this->userInfos.userName,this->userInfos.hostName,cmd[1]));
		sendMessage(NICK(this->userInfos.nickName, cmd[1]));
		this->userInfos.nickName = cmd[1];
		return (true);
	}
	return (false);
}
