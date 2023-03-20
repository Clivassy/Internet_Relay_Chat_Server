#include "client.hpp"

//
// cmd syntaxe <JOIN> <#channel1> <#channel2> ....
// cmd syntaxe <JOIN> <&channel1> <&channel2> ....
bool	Client::cmdJOIN(std::vector<std::string> &cmd)
{
	for (auto it_cmd = ++cmd.begin(); it_cmd != cmd.end(); it_cmd++)
	{
		this->server.addChannel(*it_cmd);
		this->server.getChannel(*it_cmd).addClient(*this);
	}
}
