#include "client.hpp"

bool	Client::cmdOPER(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	if (cmd.size() < 3)
	{
		sendMessage(this->getPrefix() + ERR_NEEDMOREPARAMS("OPER"));
		return (false);
	}
//	if (cmd[1] != "admis")
//	{
//		sendMessage(this->getPrefix() + ERR_USERMISMATCH);
//		return (false);
//	}
	if (cmd[2] != "admis")
	{
		sendMessage(this->getPrefix() + ERR_PASSWDMISMATCH);
		return (false);
	}
	this->userInfos.operatorMode = true;	
	sendMessage(RPL_YOUREOPER(this->userInfos.userName));
	printMode(this->userInfos.userName);
	return (true);
}
