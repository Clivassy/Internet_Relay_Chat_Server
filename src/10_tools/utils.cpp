//#include "../main.hpp"

#include "utils.hpp"

std::vector<std::string>    split(std::string toSplit, char sep)
{
	std::vector<std::string> tokenList;
	std::stringstream split(toSplit);
	std::string token;

	while (getline(split, token, sep))
	{
		tokenList.push_back(token);
	}
	return (tokenList);
}


// split arzu
//std::vector<std::string> split(std::string &s)
//{
//	std::vector<std::string> cmd;
//	std::string	str;
//	size_t	pos = 0;
////	int i = 0;

//	if (s.empty())
//		return (cmd);
//	while ((pos = s.find(" ")) != std::string::npos)
//	{
//		str = s.substr(0, pos);
//		cmd.push_back(str);
//	//	cmd[i++] = str;
//		s.erase(0, pos + 1);
//	}
//	if (!s.empty())
//		cmd.push_back(s);
//	//cmd[i++] = s;
//	cmd.push_back(std::string(""));
//	return (cmd);
//}
