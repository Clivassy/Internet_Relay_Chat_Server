#include "client.hpp"

bool	Client::cmdNAMES(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	this->server.channelList	

	std::vector<std::string> listOfChannelToShow = split(cmd[1], ",");
	std::string channel_name;

	for (std::vector<std::string>::iterator it = listOfChannelToShow.begin(); it != listOfChannelToShow.end(); it++)
	{
		channel_name = *it;
		toLowerStr(channel_name);
		for (std::map<std::string, Channel>::iterator it=this->channelList.begin(); it != this->channelList.end(); it++)
		{
			if (channel_name == it->first)
			{
				for (std::set<std::string>::iterator it_client=it->second.clientConnected.begin(); it_client != it->second.clientConnected.end(); it_client++)
				{
					std::cout << *it_client;
					if(++it_client != it->second.clientConnected.end())
					std::cout << ", ";
				--it_client;
				}
			}
		}
	}
	return (true);
}
