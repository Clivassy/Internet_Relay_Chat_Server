#include "client.hpp"

bool	Client::cmdINVITE(std::vector<std::string> &cmd)
{
    if (this->status != CONNECTED)
		return(false);
	if (cmd.size() < 3)
	{
		sendMessage(ERR_NEEDMOREPARAMS("INVITE"));
		return (false);
	}
    /* - ERRORS : 

    ERR_NOSUCHNICK : si le user n'est pas encore enregistré dans le server IRC 
    Si le channel n'existe pas => ERR_NOSUCHCHANNEL
    Si le client n'est pas dans le channel dont il a envoye l'invitation => ERR_NOTONCHANNEL
    Si le client est deja dans le channel => ERR_USERONCHANNEL
    si le channel est en mode InviteOnly et que le client n'est pas en mode operator : ERR_CHANOPRIVNEED
    ELSE 
    - > 
    When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.
	*/
    return (true);
}