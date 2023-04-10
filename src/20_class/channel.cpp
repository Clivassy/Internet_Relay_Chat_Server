#include "channel.hpp"

Channel::Channel(Server& serv, std::string channel_name):
server(serv), name(channel_name), isInviteOnly(false)
{
	this->topic = "Welcome to the channel " + channel_name + " !!";
}

Channel::~Channel(){}

bool Channel::isClientOperatorChannel(std::string name)
{
	if (this->clientOperators.count(name) > 0)
		return (true);
	return false;
}

bool	Channel::isclientConnected(std::string name)
{
	if (this->clientConnected.count(name) > 0)
		return (true);
	return false;
}

bool	Channel::isClientBanned(std::string name)
{
	if (this->clientBanned.count(name) > 0)
		return (true);
	return false;
}

// fonction prenant initialement un objet Client en argument, c'est pour ca qu'elle est codé de cette facon
void Channel::addClient(std::string name)
{
	Client& client = *(this->server.getClient(name));
	this->clientConnected.insert(client.userInfos.nickName);
	// messages de welcome envoyes selon norme IRC 1459 et http://chi.cs.uchicago.edu/chirc/assignment3.html7
	// TBD RPL_TOPIC ne marche pas (il est bien envoyé mais pas affiché coté irssi)
	std::string clientList;
	for( std::set<std::string>::iterator it = clientConnected.begin(); it != clientConnected.end(); it++ )
	{
		// TBD voir si il faut ajouter qqch en foncion du mode client
		if (this->isClientOperatorChannel(*it))
			clientList.append("@");
		clientList.append(*it);
		if (it != clientConnected.end())
			clientList.append(" ");
	}
	// messages au client se connectant
	client.sendMessage(RPL_NAMREPLY(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name) + clientList + "\r\n");
	client.sendMessage(RPL_ENDOFNAMES(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name));
	
	// annonce arrivée aux autres clients
	this->sendMessageToClients(JOIN(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name), "");
}

void Channel::addOperator(std::string name)
{
	this->clientOperators.insert(name);
}

void Channel::removeConnected(std::string name)
{
	this->clientConnected.erase(name);
}

void Channel::removeBanned(std::string name)
{
	this->clientBanned.erase(name);
}

// fonction prenant initialement un objet Client en argument, c'est pour ca qu'elle est codé de cette facon
void Channel::removeOperator(std::string name)
{
	int	isOperatorChannel = false;
	if (this->isClientOperatorChannel(name))
		isOperatorChannel = true;
		
	if (isOperatorChannel == true)
	{
		this->clientOperators.erase(name);
	}
}

// Send msg a tous les clients connectés sauf a sender
// si sender == "" -> envoie a tous les clients sur le channel
void	Channel::sendMessageToClients(std::string msg, std::string sender)
{
	for( std::set<std::string>::iterator it = clientConnected.begin(); it != clientConnected.end(); it++ )
	{
		if (this->server.getClient(*it)->userInfos.nickName != sender)
		{
			if (sendCustom(this->server.getClient(*it)->socketFd , msg.c_str(), msg.size(), 0) == -1)
				perror("error sending message: ");
		}
	}
}
