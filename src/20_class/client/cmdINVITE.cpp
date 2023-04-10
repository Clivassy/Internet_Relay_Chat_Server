#include "client.hpp"

//  Parameters: <nickname> <channel> [<comment>]
bool	Client::isValidParsingINVITE(std::vector<std::string> &cmd)
{
	if (cmd.size() < 3)
	{
		sendMessage(ERR_NEEDMOREPARAMS("INVITE"));
		return (false);
	}
  	if (!this->server.isClientExisting(cmd[1]))
  	{
		sendMessage(ERR_NOSUCHNICK(cmd[1]));
		return (false);
  	}
  	if (!this->server.isChannelExisting(cmd[2]))
	{
		sendMessage(ERR_NOSUCHCHANNEL(cmd[2]));
		return (false);
	}
	if (!this->server.getChannel(cmd[2])->second.isclientConnected(this->userInfos.nickName))
	{
		sendMessage(ERR_NOTONCHANNEL(cmd[2]));
		return (false);
	}
  	if (this->server.getChannel(cmd[2])->second.isclientConnected(cmd[1]))
  	{
		sendMessage(ERR_NOTONCHANNEL(cmd[1]));
		return (false);  
  	}
  	if (this->server.getChannel(cmd[2])->second.isInviteOnly == true)
  	{
		if (!this->server.getChannel(cmd[2])->second.isClientOperatorChannel(this->userInfos.nickName))
		{
	  		sendMessage(ERR_CHANOPRIVSNEEDED(cmd[2], this->userInfos.nickName));
	  		return(false);
		} 
  	}
  	return (true);
}

bool	Client::cmdINVITE(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return(false);
	if (!isValidParsingINVITE(cmd))
		return(false);
	std::string msgToSend = RPL_INVITING(this->userInfos.nickName, this->userInfos.userName, this->userInfos.hostName, cmd[1], cmd[2]);
	this->server.getChannel(cmd[2])->second.addClient(cmd[1]);
	sendMessage(msgToSend);
	this->server.getClient(cmd[1])->sendMessage(msgToSend);
	return(true);
}
	/* - ERRORS :
	ERR_NOSUCHNICK : si le user n'est pas encore enregistré dans le server IRC 
	Si le channel n'existe pas => ERR_NOSUCHCHANNEL
	Si le client n'est pas dans le channel dont il a envoye l'invitation => ERR_NOTONCHANNEL
	Si le client est deja dans le channel => ERR_USERONCHANNEL
	si le channel est en mode InviteOnly et que le client n'est pas en mode operator : ERR_CHANOPRIVNEED
	ELSE 
	- > 
	When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.
	*/
