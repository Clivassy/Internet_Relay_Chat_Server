//Parameters: <msgtarget> <text to be sent>

#include "client.hpp"

bool	Client::cmdPRIVMSG(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	if (cmd.size() < 3)
	{
		std::cout << ERR_NEEDMOREPARAMS("PRIVMSG");
		return (false);
	}
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
		std::string nickNameClientReceiver = cmd[1];
		if (!this->server.isClientExisting(nickNameClientReceiver))
		{
			this->sendMessage(ERR_NOSUCHNICK(nickNameClientReceiver));
			return (false);
		}
		std::string msg = cmd[2];
		std::string msgToSend = ":" + this->userInfos.nickName + " PRIVMSG " + nickNameClientReceiver + " :" + msg + "\r\n";
		this->server.getClient(nickNameClientReceiver)->sendMessage(msgToSend);

		return (true);
	}
	return (true);
}
