#include "client.hpp"

Client::Client():
bufferSize(BUFFER_SIZE), is_authentified(false)
{
}

Client::~Client(){ }

/*
	PREALABLE:
	
	vector<std::string>- > stock le split de la string user_infos avec \r

	IF (vector.size() == 3)

		STEP ONE : CAP LS 
		// split du vector[0] avec espaces -> on stocke dans un vector.
		// si vector.*begin() == CAP && vector.*begin++ == LS && vector.size() == 2
			-> CAP LS FLAG = OK -> fill data
			else
				error authentification 

		STEP TWO : NICK 
		// split du vector[0] avec espaces -> on stocke dans un vector.
		// si vector.begin() == NICK && vector.size() == 2
				-> NICK FLAG = OK -> fill data
		   else 
			error authentification 

		STEP THREE : USER 
		// split du vector[0] avec espace -> on stocke dans un vector.
		IF vector.begin() == USER && vector.size() == 5
			-> fill data : vector[1], vector[2], vector[3] = username, hostname, servername
			-> split du vector[4] avec : && stockage dans realName.

			FLAG INFOS USER RECUP => bool true
		ELSE
			error authentification 
	
	ELSE : on continue de recevoir
*/


/*
void    Client::fillDataUser(Client &client, std::string token, char sep)
{
	std::stringstream toSplit(token);
	std::string userNick;
	std::vector<std::string> userNickName;

	while (getline(toSplit, userNick, ' '))
	{
		userNickName.push_back(userNick);
	}
    // switch case 

	client.userInfos.userName = userNickName[1];
}
*/
//-- Getters
//std::string    getUserName( void ) {}
//std::string    gethostName( void ) {}
//std::string    getserverName( void ){}
//std::string    getRealName( void ) {}
//std::string    getUserMessage( void ) {}

//-- Setters
//void    setUserName( std::string userName ) {}
//void    setHostName( std::string userName ) {}
//void    setServerName( std::string userName ) {}
//void    setRealName( std::string userName ) {}
//void    setUserMessage( std::string userName ) {}
