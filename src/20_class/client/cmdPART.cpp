#include "client.hpp"

// Supprime un client ou des channels
// ERR_NOSUCHCHANNEL si le channel n'exidte pas 
// ERR_NOTONCHANNEL si le channel existe mais le client n'est pas dedans

// the user will receive a PART message from the server for each channel they have been removed from. <reason> is the reason 
// that the client has left the channel(s).
// Servers SHOULD NOT send multiple channels in this message to clients, and SHOULD distribute these multiple-channel JOIN 
// messages as a series of messages with a single channel name on each.
// return false si une erreur rencontrée
bool	Client::cmdPART(std::vector<std::string> &cmd)
{
	bool toReturn = true;

	if (cmd.size() < 2)
	{
		sendMessage(ERR_NEEDMOREPARAMS("PART"));
		return (false);
	}
	std::vector<std::string> listOfChannelToRemove = split(cmd[1], ",");
	std::string channel_name;
	std::string leavingMessage;
	std::string msgToSend;
	// loop sur tous les channels a quitter
	for (std::vector<std::string>::iterator it = listOfChannelToRemove.begin(); it != listOfChannelToRemove.end(); it++)
	{
		channel_name = *it;
		toLowerStr(channel_name);
		if (cmd.size() > 2)
			leavingMessage = "User " + this->userInfos.nickName + " leaving channel " + channel_name + " with the message " + cmd[2];
		else
			leavingMessage = "User " + this->userInfos.nickName + " leaving channel " + channel_name;
		if (!this->server.isChannelExisting(channel_name))
		{
			this->sendMessage(ERR_NOSUCHCHANNEL(channel_name));
			toReturn = false;
		}
		else if (!this->server.getChannel(channel_name)->second.isclientConnected(this->userInfos.nickName))
		{
			this->sendMessage(ERR_NOTONCHANNEL(channel_name));
			toReturn = false;
		}
		else
		{
			// TBD diffusion message depart
			// revoir message avec syntaxe specifique PART 
			msgToSend = "PRIVMSG " + channel_name + " :" + leavingMessage + "\r\n";
			this->server.getChannel(channel_name)->second.sendMessageToClients(msgToSend, "");

			this->server.getChannel(channel_name)->second.removeClient(*this);
		}
		// TBD supprimer channel vide  ?

	}

	return (toReturn);

}
