#include "client.hpp"

bool	Client::cmdPING(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	if (cmd.size() < 2)
	{
		sendMessage(this->getPrefix() + ERR_NEEDMOREPARAMS("PING"));
		return (false);
	}
	sendMessage("PONG " + cmd[1] + "\r\n");
	return (true);
}
