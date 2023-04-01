#include "client.hpp"

bool	Client::cmdPONG(std::vector<std::string> &cmd)
{
	(void)cmd;
	this->lastPongReceived = time(0);
	return (true);
}
