#include "client.hpp"

bool	Client::cmdCAP(std::vector<std::string> &cmd)
{
	if (cmd.size() == 2)
	{
		if (cmd[1].compare("LS") == 0)
		{
			this->status = COMING;
			return (true);
		}
	}
	return (false);
}