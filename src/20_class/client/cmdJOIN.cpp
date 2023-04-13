#include "client.hpp"

//comportement
// rejoind les channels dans cmd[1] avec , en separateur. 
// Par exemple si cmd[1] = "#ch1,&ch2,#ch3" > les trois channels seront rejoints
// return false si un channel n'a pas pu etre rejoint
bool	Client::cmdJOIN(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	bool toReturn = true;
	if (cmd.size() <=1)
	{
		sendMessage(ERR_NEEDMOREPARAMS("JOIN"));
		return (false);
	}
	std::vector<std::string> listOfChannelToAdd = split(cmd[1], ",");
	std::string channel_name;

	for (std::vector<std::string>::iterator it = listOfChannelToAdd.begin(); it != listOfChannelToAdd.end(); it++)
	{
		channel_name = *it;
		toLowerStr(channel_name);
		if (channel_name.size() <= 1 || !isChannelName(channel_name))
		{
			this->sendMessage(ERR_BADCHANMASK(channel_name));
			toReturn = false;
		}
		else
		{
			if (!(this->server.isChannelExisting(channel_name)))
			{
				this->server.addChannel(channel_name);
			}
			if (this->server.getChannel(channel_name)->second.isInviteOnly)
			{
				this->sendMessage(ERR_INVITEONLYCHAN(this->userInfos.nickName, channel_name));
				toReturn = false;
			}
			else if (this->server.getChannel(channel_name)->second.isClientBanned(this->userInfos.nickName))
			{
				this->sendMessage(ERR_BANNEDFROMCHAN(this->userInfos.nickName, channel_name));
				toReturn = false;
			}
			else
			{
				this->server.getChannel(channel_name)->second.addClient(this->userInfos.nickName);
				if (this->server.getChannel(channel_name)->second.clientOperators.size() == 0)
					this->server.getChannel(channel_name)->second.addOperator(this->userInfos.nickName);
			}
		}

	}
	return (toReturn);
}
