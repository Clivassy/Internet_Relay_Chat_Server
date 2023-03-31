#include "client.hpp"

bool	Client::cmdUSER(std::vector<std::string> &cmd)
{
	//------- Pré authentification pour Yann et Arzu (TEMPORAIRE) ----// 
	/*if (this->status == REGISTERED)
	{
		this->userInfos.nickName = "jbatoro_" + random_str;
		this->userInfos.userName = "jbatoro";
		this->userInfos.hostName = "jbatoro";
		this->userInfos.realName = "Julia Batoro"; 
		sendMessage(RPL_WELCOME(this->userInfos.nickName, this->userInfos.userName ,this->userInfos.hostName));
		this->status = CONNECTED;
	} */
	//------------------------------------------------------------ //
	
	/*std::cout << BOLD_PURPLE << cmd[0] << RESET << std::endl; // JULIA ::commentaire debeug
	if (cmd.size() == 1)
	{
		std::cout << BOLD_PURPLE << "CAS a gerer dans NC =>  " << cmd[0] << RESET << std::endl;
	}
	if (cmd.size() == 2) //cas de irssi ou c'est separé par deux points
	{
		std::cout << BOLD_PURPLE << "CAS a gerer dans IRSSI =>  " <<  cmd[0] << " | " << cmd[1] << RESET << std::endl;
	}*/
	if (this->status == REGISTERED and this->hasNick == true)
	{
		if (!cmd.empty())
		{
			if (cmd.size() < 5)
			{
				sendMessage(ERR_NEEDMOREPARAMS("USER"));
				return (false);
			}
			this->userInfos.userName = cmd[1];
			this->userInfos.hostName = cmd[2];
			this->userInfos.realName = cmd[4];
			sendMessage(RPL_WELCOME(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName));
			this->status = CONNECTED;
			return(true);
		}
		else if (this->status == CONNECTED)
		{
			sendMessage(ERR_ALREADYREGISTERED);
			return(false);
		}
	}
	return (false);
}
