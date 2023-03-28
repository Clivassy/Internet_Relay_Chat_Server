#include "client.hpp"

bool	Client::cmdPASS(std::vector<std::string> &cmd)
{
	if (this->status == COMING)
	{
		if (cmd.empty() or cmd.size() == 1)
		{
			sendMessage(ERR_NEEDMOREPARAMS("PASS"));
			return (false);
		}
		if (cmd.size() == 2)
		{
			if (this->status == REGISTERED)
			{
				sendMessage(ERR_ALREADYREGISTERED);
				return (true);
			}
			if (this->server.get_password().compare(cmd[1]) == 0)
			{
				//std::cout << "PASSWORD IS OK" << std::endl; //JULIA 
				this->status = REGISTERED;
				return (true);
			}
			else
			{
				//std::cout << BOLD_RED << "WRONG PASSWORD" << RESET << std::endl; //JULIA
				sendMessage(this->getPrefix() + " 464 " + this->userInfos.nickName + ERR_PASSWDMISMATCH);
				this->status = COMING;
				// this->deconnectClient(); // no need to deconnect? version Marie + William : a discuter //JULIA
				return(false);
			}
		}		
	}
	return (false);
}