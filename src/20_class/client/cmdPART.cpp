#include "client.hpp"

// /PART <channel> <reason>
bool	Client::cmdPART(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	bool toReturn = true;
	if (cmd.size() < 2)
	{
		sendMessage(ERR_NEEDMOREPARAMS("PART"));
		return (false);
	}
	std::vector<std::string> listOfChannelToRemove = split(cmd[1], ",");
	std::string channel_name;
	// loop sur tous les channels a quitter
	for (std::vector<std::string>::iterator it = listOfChannelToRemove.begin(); it != listOfChannelToRemove.end(); it++)
	{
		channel_name = *it;
		toLowerStr(channel_name);
		if (!this->server.isChannelExisting(channel_name))
		{
			this->sendMessage(ERR_NOSUCHCHANNEL(channel_name));
			toReturn = false;
		}
		else if (!this->server.getChannel(channel_name)->second.isclientConnected(this->userInfos.nickName))
		{
			this->sendMessage(ERR_NOTONCHANNEL(channel_name));
			toReturn = false;
		}
		else
		{
			if (cmd.size() > 2)
			{
				std::string leavingReason = cmd[2];
				this->server.getChannel(channel_name)->second.sendMessageToClients(PART_REASON(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, channel_name, leavingReason), "");
			}
			else
			{
				this->server.getChannel(channel_name)->second.sendMessageToClients(PART(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, channel_name), "");
			}
			this->server.getChannel(channel_name)->second.removeConnected(this->userInfos.nickName);
		}
	}
	return (toReturn);
}
