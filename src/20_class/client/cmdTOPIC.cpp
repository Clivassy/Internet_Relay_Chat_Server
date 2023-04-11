#include "client.hpp"

bool	Client::cmdTOPIC(std::vector<std::string> &cmd)
{
	(void)cmd;
	if (this->status != CONNECTED)
		return (false);
	return (true);
}
