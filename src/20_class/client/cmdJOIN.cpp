#include "client.hpp"

// INFO POUR LES CHANNELS 
// Servers MUST process the parameters of this command as lists on incoming commands from
// clients, with the first <key> being used for the first <channel>, the second <key> being
// used for the second <channel>, etc.
// While a client is joined to a channel, they receive all relevant information about that
// channel including the JOIN, PART, KICK, and MODE messages affecting the channel. They
// receive all PRIVMSG and NOTICE messages sent to the channel, and they also receive QUIT
// messages from other clients joined to the same channel (to let them know those users have
// left the channel and the network). This allows them to keep track of other channel members
// and channel modes.

// Syntaxe et fonctionnemen fonction:
// syntaxe <JOIN> <[&, #, +, !]channel1> <other> ....
// syntaxe <JOIN> <&channel1> <other> ....
// seul le 1er channel est ajouté, les other sont ignorés pour coller au comportement d'Irssi
// Erreur gerees:
// ERR_NEEDMOREPARAMS
// Erreurs non gerees
// ERR_TOOMANYCHANNELS
// ERR_BANNEDFROMCHAN : Returned to indicate that a JOIN command failed because the
// client has been banned from the channel and has not had a ban exception set for them.
// The text used in the last param of this message may vary.
// ERR_INVITEONLYCHAN : Returned to indicate that a JOIN command failed because the channel
// is set to [invite-only] mode and the client has not been invited to the channel or had
// an invite exception set for them. The text used in the last param of this message may vary.
// ERRFULL : TBD voir avec Arzu ce que c'est ?
// Note that this command also accepts the special argument of ("0", 0x30) instead of any of 
// the usual parameters, which requests that the sending client leave all channels they are
// currently connected to. The server will process this command as though the client had sent
// a PART command for each channel they are a member of.
 
// This message may be sent from a server to a client to notify the client that someone has
// joined a channel. In this case, the message <source> will be the client who is joining, and
// <channel> will be the channel which that client has joined
// Servers SHOULD NOT send multiple channels in this message to clients, and SHOULD distribute
// these multiple-channel JOIN messages as a series of messages with a single channel name on each.
 
// Des qu'il a rejoint on doit envoyer une message "<source> joined the channel" doit etre envoye
// ainsi que le topic du channel (avec RPL_TOPIC) et la liste des utilisateurs connectes (avc RPL_NAMREPLY
// suivi d'un RPL_ENDOFNAMES)

//comportement
// rejoind les channels dans cmd[1] avec , en separateur. 
// Par exemple si cmd[1] = "#ch1,&ch2,#ch3" > les trois channels seront rejoints
// return false si un channel n'a pas pu etre rejoint
bool	Client::cmdJOIN(std::vector<std::string> &cmd)
{
	if (this->status != CONNECTED)
		return (false);
	bool toReturn = true;
	if (cmd.size() <=1)
	{
		sendMessage(ERR_NEEDMOREPARAMS("JOIN"));
		return (false);
	}
	std::vector<std::string> listOfChannelToAdd = split(cmd[1], ",");
	std::string channel_name;


	for (std::vector<std::string>::iterator it = listOfChannelToAdd.begin(); it != listOfChannelToAdd.end(); it++)
	{
		channel_name = *it;
		toLowerStr(channel_name);
		if (this->server.getChannel(channel_name)->second.isInviteOnly)
		{
			this->sendMessage(ERR_INVITEONLYCHAN(this->userInfos.nickName, channel_name));
			toReturn = false;
		}
		if (channel_name.size() <= 1 || !isChannelName(channel_name))
		{
			this->sendMessage(ERR_BADCHANMASK(channel_name));
			toReturn = false;
		}
		if (!(this->server.isChannelExisting(channel_name)))
		{
			this->server.addChannel(channel_name);
			this->server.getChannel(channel_name)->second.addOperator(this->userInfos.nickName);
		}
		this->server.getChannel(channel_name)->second.addClient(this->userInfos.nickName);
	}
	return (toReturn);
}
