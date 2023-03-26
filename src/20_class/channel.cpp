#include "channel.hpp"

Channel::Channel( Server& serv, std::string channel_name):
server(serv), name(channel_name), isInviteOnly(false)
{
	this->topic = "Welcome to the channel '" + channel_name + "' !!";
}

Channel::~Channel( void )
{

}

bool	Channel::isClientBanned(std::string name)
{
	if (this->clientBanned.count(name) >= 1)
		return (true);
	return false;
}


// Ajouter le fd du clien qui se connecte au serveur dans la liste des clients connectés
void Channel::addClient(Client& client)
{
	if (isClientBanned(client.userInfos.nickName))
	{
		ERR_BANNEDFROMCHAN(client.userInfos.nickName, this->name);
		return;
	}
		this->clientConnected.insert(client.userInfos.nickName);
		// messages de welcome envoyes selon norme IRC 1459 et http://chi.cs.uchicago.edu/chirc/assignment3.html7
		// TBD a confirmer ce qu'il faut afficher
		client.sendMessage(RPL_TOPIC(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name, this->topic));
		std::string clientList;
		for( std::set<std::string>::iterator it = clientConnected.begin(); it != clientConnected.end(); it++ )
		{
			// TBD voir si il faut ajouter qqch en foncion du mode client
			clientList.append(*it);
			if (it != clientConnected.end())
				clientList.append(" ");
		}
		client.sendMessage(RPL_NAMREPLY(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name) + clientList + "\r\n");
		client.sendMessage(RPL_ENDOFNAMES(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name));
		
		// annonce arrivée aux autres clients
		this->sendMessageToClients(JOIN(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name), "");
}

// Send msg a tous les clients connectés sauf a sender
// si sender == "" -> envoie a tous les clients sur le channel
void    Channel::sendMessageToClients(std::string msg, std::string sender)
{
	for( std::set<std::string>::iterator it = clientConnected.begin(); it != clientConnected.end(); it++ )
	{
		if (this->server.getClient(*it)->userInfos.nickName != sender)
		{
			std::string pingmessaage = "PING\r\n"; // ping avant sinon 1er message non recu apres inactivité TBD a sup qd ca marchera sans
			send(this->server.getClient(*it)->socketFd , pingmessaage.c_str(), pingmessaage.size(), 0);
			if (send(this->server.getClient(*it)->socketFd , msg.c_str(), msg.size(), 0) == -1)
				perror("error sending message: ");
		}

	}   

}




