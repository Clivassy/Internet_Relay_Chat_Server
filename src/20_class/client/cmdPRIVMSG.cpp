//Parameters: <msgtarget> <text to be sent>

#include "client.hpp"


// The PRIVMSG command is used to send private messages between users, as well as to send messages to
// channels. <target> is the nickname of a client or the name of a channel.

//If <target> is a channel name and the client is banned and not covered by a ban exception, the 
//message will not be delivered and the command will silently fail.
// Si le message ne peut pas etre envoye ERR_CANNOTSENDTOCHAN
// If <target> is a channel name, it may be prefixed with one or more channel membership prefix 
//character (@, +, etc) and the message will be delivered only to the members of that channel with 
//the given or higher status in the channel. Servers that support this feature will list the prefixes
//which this is supported for in the STATUSMSG RPL_ISUPPORT parameter, and this SHOULD NOT be 
//attempted by clients unless the prefix has been advertised in this token.
// If <target> is a user and that user has been set as away, the server may reply with an RPL_AWAY 
//(301) numeric and the command will continue.

// When the PRIVMSG message is sent from a server to a client and <target> starts with a dollar character ('$', 0x24), the message is a broadcast sent to all clients on one or multiple servers.

bool	Client::cmdPRIVMSG(std::vector<std::string> &cmd)
{
	std::cout << "size: " << cmd.size() << std::endl;
	std::cout << "0: " << cmd[0] << std::endl;
	std::cout << "1: " << cmd[1] << std::endl;
	// TBD a decommenter ??
	//if (cmd.size() < 3)
	//{
	//	std::cout << ERR_NEEDMOREPARAMS("PRIVMSG");
	//	return (false);
	//}

	if (isChannelName(cmd[1]))
	{
		std::string channelName = cmd[1];
		std::string msg = cmd[2];
		Channel& channel = this->server.getChannel(channelName)->second;
		if (!this->server.isChannelExisting(channelName))
		{
			this->sendMessage(ERR_NOSUCHNICK(channelName));
			return (false);
		}
		if (channel.isClientBanned(this->userInfos.nickName))
		{
			// TBD a tester quand ban (commande KICK) fait
			this->sendMessage(ERR_BANNEDFROMCHAN(this->userInfos.nickName, channel.name));
			return (false);
		}
		if (!channel.isclientConnected(this->userInfos.nickName))
		{
			this->sendMessage(ERR_CANNOTSENDTOCHAN(this->userInfos.nickName, channelName));
			return (false);
		}
		std::string msgToSend = ":" + this->userInfos.nickName + " PRIVMSG " + channelName + " :" + msg + "\r\n";
		this->server.getChannel(channelName)->second.sendMessageToClients(msgToSend, this->userInfos.nickName);
		return (true);
	}
	else
	{
		// TBD envoyer un message direct au client cmd[1]
		return (true);
	}
	return (true);
}
