#include "client.hpp"

//  Parameters: <channel> <user> [<comment>]
bool	Client::isValidParsingKICK(std::vector<std::string> &cmd)
{
	if (cmd.size() < 3)
	{
		sendMessage(ERR_NEEDMOREPARAMS("KICK"));
		return (false);
	}
	if (!this->server.isChannelExisting(cmd[1]))
	{
		sendMessage(ERR_NOSUCHCHANNEL(cmd[1]));
		return (false);
	}
	if (!this->server.getChannel(cmd[1])->second.isclientConnected(this->userInfos.nickName))
	{
		sendMessage(ERR_NOTONCHANNEL(cmd[1]));
		return (false);
	}
	if (!this->server.getChannel(cmd[1])->second.isClientOperatorChannel(this->userInfos.nickName)) 
	{
		sendMessage(ERR_CHANOPRIVSNEEDED(cmd[1], this->userInfos.nickName));
		return (false);
	}
	return(true);
}

bool	Client::cmdKICK(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return(false);
	if (!isValidParsingKICK(cmd))
		return(false);
	this->server.getChannel(cmd[1])->second.removeClient(cmd[2]);
	return (true);
}