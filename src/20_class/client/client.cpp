#include "client.hpp"

Client::Client(Server& serv):
server(serv), bufferSize(BUFFER_SIZE), status(WAITING),hasNick(false), online(true)
{ 
	this->userInfos.invisibleMode = true;
	this->userInfos.operatorMode = false;
	this->lastPingSent = time(0);
	this->lastPongReceived = time(0);
	
}

Client::~Client(){ }

std::string	Client::getPrefix( void )
{
	return(":" + this->userInfos.nickName + "!" + this->userInfos.userName + "@" + this->userInfos.hostName);
}

Client& Client::operator=(const Client&other)
{
	// userInfos 
	this->userInfos.nickName = other.userInfos.nickName;
	this->userInfos.userName = other.userInfos.userName;
	this->userInfos.hostName = other.userInfos.hostName;
	this->userInfos.serverName = other.userInfos.serverName;
	this->userInfos.realName = other.userInfos.realName;
	this->userInfos.invisibleMode = other.userInfos.invisibleMode;
	this->userInfos.operatorMode = other.userInfos.operatorMode;

	// Other Attributes
	this->socketFd = other.socketFd;
	this->clientAddr = other.clientAddr;
	this->clientSize = other.clientSize;
	for (int i = 0; i < this->bufferSize; i++)
	{
		this->buffer[i] = other.buffer[i];
	}
	this->cmd = other.cmd;
	this->status = other.status;
	this->hasNick = other.hasNick;
	this->online = other.online;
	this->lastPingSent = other.lastPingSent;
	this->lastPongReceived = other.lastPongReceived;
	
	return (*this);
}

void	Client::ping()
{
	sendMessage(PING(this->userInfos.hostName));
}

Client::User Client::getUserInfo() const
{
	return (this->userInfos);
}

int Client::getSocketFd() const
{
	return (this->socketFd);
}
