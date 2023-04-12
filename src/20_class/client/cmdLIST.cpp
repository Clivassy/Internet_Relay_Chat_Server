#include "client.hpp"

bool	Client::cmdLIST(std::vector<std::string> &cmd)
{
	(void)cmd;
	if (this->status != CONNECTED)
		return (false);
	return (true);

}
