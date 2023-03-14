#ifndef SERVER_HPP
# define SERVER_HPP

# include "../main.hpp"

class Server
{
	protected:
		int			port;
		std::string	password;

		const int			bufferSize;
		char				buffer[BUFFER_SIZE];
		int					socketFd;
		int					serverFd;
		struct sockaddr_in	serverAddr;
		std::vector<pollfd> fd_listened;

		//TBD 
		// ajouter tableau de client
		// tableau de pollfd


	public:
		Server();
		void set_port(int port);
		void set_password(std::string password);
		void init();
		void run();
		void terminate();
};

#endif 
