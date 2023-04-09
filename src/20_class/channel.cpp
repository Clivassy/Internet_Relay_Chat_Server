#include "channel.hpp"

Channel::Channel(Server& serv, std::string channel_name):
server(serv), name(channel_name), isInviteOnly(false)
{
	this->topic = "Welcome to the channel " + channel_name + " !!";
}

Channel::~Channel( void )
{

}

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
	// TBD a confirmer ce qu'il faut afficher
	// TBD RPL_TOPIC ne marche pas (il est bien envoyé mais pas affiché coté irssi)
	//std::string pingmessaage = "PING\r\n"; // TBD ping avant sinon 1er message non recu apres inactivité -> a sup qd ca marchera sans
	//sendCustom(client.socketFd , pingmessaage.c_str(), pingmessaage.size(), 0);client.sendMessage(RPL_TOPIC(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name, this->topic));
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
	client.sendMessage(RPL_NAMREPLY(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name) + clientList + "\r\n");
	client.sendMessage(RPL_ENDOFNAMES(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name));
	
	// annonce arrivée aux autres clients
	this->sendMessageToClients(JOIN(client.userInfos.nickName, client.userInfos.userName, client.userInfos.hostName, this->name), "");
}

void Channel::addOperator(std::string name)
{
		this->clientOperators.insert(name);
}

// fonction prenant initialement un objet Client en argument, c'est pour ca qu'elle est codé de cette facon
void Channel::removeConnected(std::string name)
{
	Client& client = *(this->server.getClient(name));
	this->clientConnected.erase(client.userInfos.nickName);
}

void Channel::removeBanned(std::string name)
{
	Client& client = *(this->server.getClient(name));
	this->clientBanned.erase(client.userInfos.nickName);
}

void Channel::removeOperator(std::string name)
{
	int	isOperatorChannel = false;
	if (this->isClientOperatorChannel(name))
		isOperatorChannel = true;
		
	// si le user est channelOperator et que la liste de channelOperator est == 1
	if (isOperatorChannel == true and this->clientOperators.size() == 1)
	{
		std::cout << BOLD_RED << "CHANGE OPERATOR CHANNEL LIST" << RESET << std::endl;
		this->clientOperators.erase(name);
 		std::string clientName = *(this)->clientConnected.begin();
		this->clientOperators.insert(clientName);
	}
	Client& client = *(this->server.getClient(name));
	this->clientOperators.erase(client.userInfos.nickName);
}

// Send msg a tous les clients connectés sauf a sender
// si sender == "" -> envoie a tous les clients sur le channel
void    Channel::sendMessageToClients(std::string msg, std::string sender)
{
	for( std::set<std::string>::iterator it = clientConnected.begin(); it != clientConnected.end(); it++ )
	{
		if (this->server.getClient(*it)->userInfos.nickName != sender)
		{
			//std::string pingmessaage = "PING\r\n"; // TBD ping avant sinon 1er message non recu apres inactivité -> a sup qd ca marchera sans
			//sendCustom(this->server.getClient(*it)->socketFd , pingmessaage.c_str(), pingmessaage.size(), 0);// TBD sup avec ping
			if (sendCustom(this->server.getClient(*it)->socketFd , msg.c_str(), msg.size(), 0) == -1)
				perror("error sending message: ");
		}
	}   

}




