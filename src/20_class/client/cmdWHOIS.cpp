#include "client.hpp"

void	Client::printMode()
{
	if (this->userInfos.invisibleMode)
		sendMessage(getPrefix() + RPL_UMODEIS(this->userInfos.nickName, "+", "i"));
	if (this->userInfos.operatorMode)
		sendMessage(getPrefix() + RPL_UMODEIS(this->userInfos.nickName, "+", "o"));
}

bool	Client::cmdWHOIS(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	if (cmd.size() < 2)
	{
		sendMessage(this->getPrefix() + ERR_NONICKNAMEGIVEN);
		return (false);
	}
	sendMessage(RPL_WHOISUSER(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, this->userInfos.realName));
	printMode();
	return (true);
}
