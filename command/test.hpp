#ifndef TEST_HPP
#define TEST_HPP


bool	check_command(std::string &s, std::string &pw, int *cnct);
bool	PASS(std::vector<std::string> &cmd, std::string &pw, int *cnct);
bool	NICK(std::vector<std::string> &cmd);
bool	USER(std::vector<std::string> &cmd);
bool	PING(std::vector<std::string> &cmd);
bool	PONG(std::vector<std::string> &cmd);
bool	OPER(std::vector<std::string> &cmd);
bool	QUIT(std::vector<std::string> &cmd);
bool	JOIN(std::vector<std::string> &cmd);
bool	PART(std::vector<std::string> &cmd);
bool	PRIVMSG(std::vector<std::string> &cmd);
bool	ERROR(std::vector<std::string> &cmd);

#endif
