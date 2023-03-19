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

std::vector<std::string> split(std::string str, std::string separator)
{
	std::vector<std::string> output;
	std::string	sub_str;
	size_t	pos = 0;

	if (str.empty())
		return (output);
	while ((pos = str.find(separator)) != std::string::npos)
	{
		sub_str = str.substr(0, pos);
		output.push_back(sub_str);
		str.erase(0, pos + 1);
	}
	if (!str.empty())
		output.push_back(str);
	output.push_back(std::string(""));
	return (output);
}

void clear_str(char* str, size_t size)
{
	size_t i = 0;
	while (i < size)
	{
		str[i] = 0;
		i++;
	}
}

// take a string, remove and return the part before the first occurence off /r/n
std::string pop_command(std::string& cmd)
{
	std::vector<std::string> output;
	if (cmd.size() == 0)
		return (cmd);
	if (cmd.find("\r\n") == 0)
		return (cmd);
	output = split(cmd, "\r\n");
	cmd.erase(0, output[0].size() + 2); // size + 2 pour enlever "\r\n"
	return (output[0]);
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
