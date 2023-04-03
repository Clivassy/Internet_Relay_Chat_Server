#include "client.hpp"

	//The difference between NOTICE and PRIVMSG is that automatic replies must never be sent in response to a NOTICE message.This rule also applies to servers – they must not send any error back to the client on receipt of a NOTICE command.
	// SI j'ai bien compris un NOTICE est un PRIVMSG sans message d'erreur


bool	Client::cmdNOTICE(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	if (cmd.size() < 3)
	{
		return (false);
	}
	std::string nickNameClientReceiver = cmd[1];
	if (!this->server.isClientExisting(nickNameClientReceiver))
	{
		return (false);
	}
	std::string msg = cmd[2];
	std::string msgToSend = ":" + this->userInfos.nickName + " NOTICE " + nickNameClientReceiver + " :" + msg + "\r\n";
	//std::string pingmessaage = "PING\r\n"; // TBD ping avant sinon 1er message non recu apres inactivité -> a sup qd ca marchera sans
	//sendCustom(this->server.getClient(nickNameClientReceiver)->socketFd , pingmessaage.c_str(), pingmessaage.size(), 0); // TBD sup avec ping
	this->server.getClient(nickNameClientReceiver)->sendMessage(msgToSend);
	return (true);
}
