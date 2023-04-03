#include "client.hpp"

bool	Client::cmdPONG(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	(void)cmd;
	this->lastPongReceived = time(0);
	return (true);
}
