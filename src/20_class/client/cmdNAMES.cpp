#include "client.hpp"

bool	Client::cmdNAMES(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
//	this->server.channelList
	std::vector<std::string> listOfChannelToShow = split(cmd[0], ",");
	if (cmd.size() != 1)
		std::vector<std::string> listOfChannelToShow = split(cmd[1], ",");

	std::string channel_name;

	for (std::vector<std::string>::iterator it = listOfChannelToShow.begin(); it != listOfChannelToShow.end(); it++)
	{
		channel_name = *it;
		toLowerStr(channel_name);
		for (std::map<std::string, Channel>::iterator it_channel=this->server.channelList.begin(); it_channel != this->server.channelList.end(); it_channel++)
		{
			std::cout << "channel_names " << channel_name << std::endl;
			if (channel_name == "names" || channel_name == it_channel->first)
			{
		//		this->sendMessage(it_channel->first + "\n");
				for (std::set<std::string>::iterator it_client=it_channel->second.clientConnected.begin(); it_client != it_channel->second.clientConnected.end(); it_client++)
				{
		//			this->sendMessage("Bouhhh\n");
		//			std::vector<Client>::iterator cl = this->server.getClient(*it_client);
		//			this->sendMessage(RPL_NAMREPLY(cl->userInfos.nickName, cl->userInfos.userName, cl->userInfos.hostName, channel_name));
					this->sendMessage(RPL_NAMREPLY2(*it_client, it_channel->first));
				}
			}
		}
	}
	return (true);
}
