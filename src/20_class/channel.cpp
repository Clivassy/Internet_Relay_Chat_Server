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
}

// recevoir une string et send a tous les clients connectés
void    Channel::sendMessageToClients( std::string msg )
{
	//STEP  Parcourir le tableau de clientConnected from begin() to end() -> send(message) a chaque client.
	for( std::set<std::string>::iterator it = clientConnected.begin(); it != clientConnected.end(); it++ )
	{
		std::string fomatedMessage = "PRIVMSG #" + this->name + " :" + msg + "\r\n"; // TBD faire fonction de creation message createChannelMsg(channelName, msg)
		//std::cout << "msg: " << fomatedMessage << " - fd: " << this->server.getClient(*it)->socketFd << std::endl;
		send(this->server.getClient(*it)->socketFd , fomatedMessage.c_str(), fomatedMessage.size(), 0);
	}   


}




