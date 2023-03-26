#include "client.hpp"

// The NICK message may be sent from the server to clients to acknowledge their NICK command was successful,  // ARZU
// and to inform other clients about the change of nickname. // ARZU
// In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.}// ARZU
bool	Client::cmdNICK(std::vector<std::string> &cmd)
{
	if (this->status == REGISTERED or this->status == CONNECTED)
	{
		if (cmd.size() == 1)
		{
			sendMessage(ERR_NONICKNAMEGIVEN);
			return (false);
		}
		if (cmd.size() > 2)
		{
			sendMessage(ERR_ERRONEUSNICKNAME(cmd[1])); 
			return (false);
		}
	if (this->status == REGISTERED )
	{
		if (cmd.size() == 2)
		{

			this->userInfos.nickName = cmd[1];
			this->hasNick = true;
			return (true);
		}
	}
	}
	if (this->status == CONNECTED)
	{
		std::cout << BOLD_PURPLE << "NICKNAME CHANGE" << RESET << std::endl;
		// FIX : affichage x2 du changement de nickname // OU Pas d'affichage du tout 
		//sendMessage(NICK_INFORM(this->userInfos.nickName,this->userInfos.userName,this->userInfos.hostName,cmd[1]));
		sendMessage(NICK(this->userInfos.nickName, cmd[1]));
		this->userInfos.nickName = cmd[1];
		return (true);
	}
	return (false);
}