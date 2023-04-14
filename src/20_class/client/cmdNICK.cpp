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
				sendMessage(ERR_NICKNAMEINUSE(cmd[1]));
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
			sendMessage(ERR_NICKNAMEINUSE(cmd[1]));
			return (false);
		}
		sendMessage(NICK(this->userInfos.nickName, cmd[1]));
		for (std::map<std::string, Channel>::iterator it=this->server.channelList.begin(); it != this->server.channelList.end(); it++)
		{
			for (std::set<std::string>::iterator it_client=it->second.clientConnected.begin(); it_client != it->second.clientConnected.end(); it_client++)
			{
				if (*it_client == this->userInfos.nickName)
				{
					it->second.clientConnected.erase(it_client);
					it->second.clientConnected.insert(cmd[1]);
				}
			}
			for (std::set<std::string>::iterator it_client=it->second.clientOperators.begin(); it_client != it->second.clientOperators.end(); it_client++)
			{
				if (*it_client == this->userInfos.nickName)
				{
					it->second.clientOperators.erase(it_client);
					it->second.clientOperators.insert(cmd[1]);
				}
			}
			for (std::set<std::string>::iterator it_client=it->second.clientBanned.begin(); it_client != it->second.clientBanned.end(); it_client++)
			{
				if (*it_client == this->userInfos.nickName)
				{
					it->second.clientBanned.erase(it_client);
					it->second.clientBanned.insert(cmd[1]);
				}
			}
		}
		this->userInfos.nickName = cmd[1];
		return (true);
	}
	this->online = false;
	return (false);
}
