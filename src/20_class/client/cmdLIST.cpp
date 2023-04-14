#include "client.hpp"

bool	Client::cmdLIST(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	
	std::vector<std::string> listOfChannelToShow;
	if (cmd.size() != 1)
		listOfChannelToShow = split(cmd[1], ",");
	else
		listOfChannelToShow = split(cmd[0], ",");
	
	std::string channel_name;

	for (std::vector<std::string>::iterator it = listOfChannelToShow.begin(); it != listOfChannelToShow.end(); it++)
	{
		channel_name = *it;
		toLowerStr(channel_name);
		for (std::map<std::string, Channel>::iterator it_channel=this->server.channelList.begin(); it_channel != this->server.channelList.end(); it_channel++)
		{
			if (channel_name == "list" || channel_name == it_channel->first)
			{
					this->sendMessage(this->getPrefix() + RPL_LIST(it_channel->first, this->server.getChannel(it_channel->first)->second.topic));
			}
		}
		this->sendMessage(this->getPrefix() + RPL_LISTEND);
	}
	return (true);
}
