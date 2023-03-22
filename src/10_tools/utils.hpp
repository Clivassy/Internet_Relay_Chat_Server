#ifndef UTILS_HPP
# define UTILS_HPP

# include <vector>
# include <map>
# include <set>
# include <sstream>
# include <string>
#include <iostream>

std::vector<std::string>    split(std::string toSplit, char sep);
std::vector<std::string> split(std::string str, std::string separator);
void clear_str(char* str, size_t size);
void replace_rn_by_n(std::string& str);
std::string pop_command(std::string& cmd);
template<typename T>
void print_vector(std::vector<T> vec);
//template<typename key, typename T>
//void print_map(std::map<key, T> map);
//template<typename key>
//void print_set(std::set<key> set);
void test();

#endif
