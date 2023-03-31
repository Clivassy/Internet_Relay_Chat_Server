#include "client.hpp"

//  Parameters: <channel> <user> [<comment>]
//checker status == CONNECTED 
bool	Client::cmdKICK(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return(false);
	if (cmd.size() < 3)
	{
		sendMessage(ERR_NEEDMOREPARAMS("KICK"));
		return (false);
	}
	// supprime le user du channel
	return (true);
}