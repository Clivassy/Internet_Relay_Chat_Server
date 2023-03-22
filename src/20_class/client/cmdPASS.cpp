/*#include "client.hpp"

bool	Client::cmdPASS(std::vector<std::string> &cmd)
{
	int pos = 0;
	int tmp_pos = 0;

	if (cmd[1].empty())
	{
		sendMessage(ERR_NEEDMOREPARAMS("PASS"));
		return (false);
	}
	// Check avec espace
	if ((pos = this->server.get_password().find(" ")) == (int)std::string::npos)
	{
		if (cmd[1] != this->server.get_password())
		{
			sendMessage(ERR_PASSWDMISMATCH);
			return (false);
		}
	}
	else
	{
		for (int i = 1; i < (int)cmd.size(); i++)
		{
			if (cmd[i] != this->server.get_password().substr(tmp_pos, pos))
			{
				sendMessage(ERR_PASSWDMISMATCH);
				return (false);
			}
			tmp_pos = pos + 1;
		}
	}
	this->isConnected = true;
	sendMessage("Connection established \r\n");
	return (true);
}*/