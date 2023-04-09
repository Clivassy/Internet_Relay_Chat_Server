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

void replace_rn_by_n(std::string& str)
{
	while(str.find("\r\n") != std::string::npos)
	{
		str.replace(str.find("\r\n"),  2, "\n");
	}
	
}

void toUpperStr(std::string& str)
{
	for(std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		*it = toupper(*it);
	}
}

void toLowerStr(std::string& str)
{
	for(std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		*it = tolower(*it);
	}
}

// take a string, remove and return the part before the first occurence off /n
std::string pop_command(std::string& cmd)
{
	std::vector<std::string> output;
	if (cmd.size() == std::string::npos)
		return (cmd);
	if (cmd.find("\n") == std::string::npos)
		return (cmd);
	output = split(cmd, "\n");
	cmd.erase(0, output[0].size() + 1); // size + 1 pour enlever "\n"
	return (output[0]);
}

// regles de nommage dans channel.hpp
bool isChannelFlag(char flag)
{
	if (flag == '&' || flag == '#' || flag == '+' || flag == '?')
		return (true);
	return (false);
}

// regles de nommage dans channel.hpp
bool isChannelName(std::string str)
{
	if (str.size() == 0 || str.size() == 1 || str.size() > 50)
		return (false);
	if (str.find(" ") !=  std::string::npos || str.find(7) !=  std::string::npos || str.find(',') !=  std::string::npos)
		return (false);
	if (!isChannelFlag(str[0]))
		return (false);
	return (true);
}

ssize_t sendCustom(int sockfd, const void *buf, size_t len, int flags)
{
	std::string msg;
	const char* pt;
	pt = static_cast <const char*> (buf);
	for (size_t i = 0; i < len ; i++)
	{
		if (pt[i] == '\r')
		{
			msg.push_back('\\');
			msg.push_back('r');
		}
		else if (pt[i] == '\n')
		{
			msg.push_back('\\');
			msg.push_back('n');
		}
		else
			msg.push_back(pt[i]);
	}
	std::cout << BOLD_GREEN << "=> " << BOLD_YELLOW << "Message sent to fd " << sockfd << BOLD_BLACK << ": -->" << YELLOW << msg << BOLD_BLACK << "<--" << RESET << std::endl;
	return(send(sockfd, buf, len, flags));
}

template<typename T>
void print_vector(std::vector<T> vec)
{
	std::cout << "[";
	for (typename std::vector<T>::iterator it=vec.begin(); it != vec.end(); it++)
	{
		std::cout << *it;
		if(it + 1 != vec.end())
			std::cout << ", ";


	}
	std::cout << "]" << std::endl;
}

void print_vector_client(std::vector<Client> vec)
{
	std::cout << "[";
	for (std::vector<Client>::iterator it=vec.begin(); it != vec.end(); it++)
	{
		std::cout << it->userInfos.nickName;
		if(it + 1 != vec.end())
			std::cout << ", ";


	}
	std::cout << "]" << std::endl;
}

template<typename key, typename T>
void print_map(std::map<key, T> map)
{
	std::cout << "[";
	for (typename std::map<key, T>::iterator it=map.begin(); it != map.end();)
	{
		std::cout << (it->first);
		if(++it != map.end())
			std::cout << ", ";
	}
	std::cout << "]" << std::endl;
}

template<typename key>
void print_set(std::set<key> set)
{
	std::cout << "[";
	for (typename std::set<key>::const_iterator it=set.begin(); it != set.end();)
	{
		std::cout << (*it);
		if(++it != set.end())
			std::cout << ", ";
	}
	std::cout << "]" << std::endl;
}
