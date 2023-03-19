#ifndef UTILS_HPP
# define UTILS_HPP

# include <vector>
# include <sstream>
# include <string>

std::vector<std::string>    split(std::string toSplit, char sep);
std::vector<std::string> split(std::string str, std::string separator);
void clear_str(char* str, size_t size);
std::string pop_command(std::string& cmd);

#endif
