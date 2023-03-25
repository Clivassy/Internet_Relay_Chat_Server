#include "channel.hpp"

Channel::Channel( Server& serv, std::string channel_name):
server(serv), name(channel_name)
{
    
}

Channel::~Channel( void )
{

}

// Ajouter le fd du clien qui se connecte au serveur dans la liste des clients connectés
void Channel::addClient(Client& client)
{
	clientConnected.insert(client.userInfos.nickName);
			// TBD ajout message annonce arrivee client
	
}

// Send msg a tous les clients connectés sauf a sender
// si sender == "" -> envoie a tous les clients sur le channel
void    Channel::sendMessageToClients(std::string msg, std::string sender)
{
	for( std::set<std::string>::iterator it = clientConnected.begin(); it != clientConnected.end(); it++ )
	{
		std::string fomatedMessage = ":" + sender + " PRIVMSG " + this->name + " :" + msg + "\r\n"; // TBD faire fonction de creation message createChannelMsg(channelName, msg)
		if (this->server.getClient(*it)->userInfos.nickName != sender)
		{
			//std::cout << BOLD_YELLOW << "msg: " << YELLOW << fomatedMessage << RESET << std::endl;
			//std::cout << BOLD_YELLOW  " - sent to fd " << YELLOW << this->server.getClient(*it)->socketFd << RESET << std::endl;
			std::string pingmessaage = "PING\r\n"; // ping avant sinon 1er message non recu apres inactivité TBD a sup qd ca marchera sans
			send(this->server.getClient(*it)->socketFd , pingmessaage.c_str(), pingmessaage.size(), 0);
			if (send(this->server.getClient(*it)->socketFd , fomatedMessage.c_str(), fomatedMessage.size(), 0) == -1)
				perror("error sending message: ");
		}

	}   


}




