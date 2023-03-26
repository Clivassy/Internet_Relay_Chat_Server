#include "client.hpp"

bool	Client::cmdMODE(std::vector<std::string> &cmd)
{
	if (cmd.size() < 2)
	{
		sendMessage(ERR_NEEDMOREPARAMS("MODE"));
		return (false);
	}
	return (true);
}