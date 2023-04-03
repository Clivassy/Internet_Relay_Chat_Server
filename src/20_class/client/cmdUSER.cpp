#include "client.hpp"

bool	Client::cmdUSER(std::vector<std::string> &cmd)
{
	if (this->status == REGISTERED and this->hasNick == true)
	{
		if (!cmd.empty())
		{
			if (cmd.size() < 5)
			{
				sendMessage(ERR_NEEDMOREPARAMS("USER"));
				return (false);
			}
			this->userInfos.userName = cmd[1];
			this->userInfos.hostName = cmd[2];
			this->userInfos.realName = cmd[4];
			sendMessage(RPL_WELCOME(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName));
			this->status = CONNECTED;
			return(true);
		}
		else if (this->status == CONNECTED)
		{
			sendMessage(ERR_ALREADYREGISTERED);
			return(false);
		}
	}
	return (false);
}
