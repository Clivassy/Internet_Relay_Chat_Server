#include "client.hpp"

// Parameters: <channel> [ <topic> ]
bool	Client::cmdTOPIC(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	if (cmd.size() < 2)
	{
		sendMessage(ERR_NEEDMOREPARAMS("TOPIC"));
		return (false);
	}
	std::string channel = cmd[1];
	if(!this->server.isChannelExisting(channel))
		return (false);
	std::string currentTopic = this->server.getChannel(channel)->second.topic;

	// si 1 parametre: on renvoie le nom du topic
	if (cmd.size() == 2)
	{
		if (currentTopic == "")
			this->sendMessage(RPL_NOTOPIC(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, channel));
		else
			this->sendMessage(RPL_TOPIC(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, channel, currentTopic));
		return (true);
	}
	// si 2 parametres: changement du topic si le user est operator (les autres parametres sont ignores)
	else
	{
		if (!this->server.getChannel(channel)->second.isClientOperatorChannel(this->userInfos.nickName))
		{
			this->sendMessage(ERR_CHANOPRIVSNEEDED(channel, this->userInfos.nickName));
			return (false);
		}
		std::string topicToSet = cmd[2];
		this->server.getChannel(channel)->second.topic = cmd[2];
		this->server.getChannel(channel)->second.sendMessageToClients(RPL_TOPIC(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, channel, topicToSet), "");
		return (true);
	}
}

