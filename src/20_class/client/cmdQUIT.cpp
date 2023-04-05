#include "client.hpp"

bool	Client::cmdQUIT(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	this->server.removeClient(this->userInfos.nickName);
	if (cmd.size() == 1)
	{
		sendMessage(QUIT(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName));
		sendOtherClient(QUIT(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName));
	}
	else
	{
		sendMessage(QUIT_REASON(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, cmd[1]));	
		sendOtherClient(QUIT_REASON(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, cmd[1]));	
	}
	return (true);
}
