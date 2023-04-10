#ifndef UTILS_HPP
# define UTILS_HPP

# include <vector>
# include <map>
# include <set>
# include <sstream>
# include <string>
# include <iostream>
# include "colors.hpp"
# include <sys/types.h>
# include <sys/socket.h>
# include "../20_class/client/client.hpp"

class Client;

std::vector<std::string>	split(std::string toSplit, char sep);
std::vector<std::string> split(std::string str, std::string separator);
void clear_str(char* str, size_t size);
void replace_rn_by_n(std::string& str);
void toUpperStr(std::string& str);
void toLowerStr(std::string& str);
std::string pop_command(std::string& cmd);
bool isChannelFlag(char flag);
bool isChannelName(std::string str);
ssize_t sendCustom(int sockfd, const void *buf, size_t len, int flags);
template<typename T>
void print_vector(std::vector<T> vec);
void print_vector_str(std::vector<std::string> vec);
void print_vector_client(std::vector<Client> vec);
//template<typename key, typename T>
//void print_map(std::map<key, T> map);
//template<typename key>
//void print_set(std::set<key> set);

#endif
