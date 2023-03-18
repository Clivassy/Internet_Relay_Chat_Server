#include "channel.hpp"

Channel::Channel( Server& serv ) : server(serv)
{
    
}

Channel::~Channel( void )
{

}

// Ajouter le fd du clien qui se connecte au serveur dans la liste des clients connectés
void    Channel::addClientToChannel( void )
{
	
}

// recevoir une string et send a tous les clients connectés
void    Channel::sendMessageToClients( std::string msg )
{
	//STEP  Parcourir le tableau de clientConnected from begin() to end() -> send(message) a chaque client.
	for( std::set<int>::iterator it = clientConnected.begin(); it != clientConnected.end(); it++ )
	{
		send(*it, msg.c_str(), msg.size(), 0);
	}   

}

// pushback : le message dans messagesHistory.
void	Channel::addMsgToHistory(std::string msg)
{
	this->messagesHistory.push_back(msg);
}
