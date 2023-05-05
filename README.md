<h1 align="center">IRC Server in C++98</h1>

# Overview
This is an IRC server implementation in C++ 98, which allows users to communicate with each other via text messages over the internet. It supports group conversations through chat channels and direct messages between two individuals.

# Getting Started

## Prerequisites
To run this program, you will need:

- A Unix-like operating system (Linux, macOS)<br>
- g++ compiler

## Installation
1- Clone the repository: git clone ``https://github.com/Clivassy/ft_irc``<br>
2- ``cd ft_irc`` <br>
3- Compile the code using the provided Makefile: ``make``<br>
3- Run the server with the following command: ``./ircserv <port> <password>`` <br>
<i>Note: Replace <port> with the port number on which the server should listen for incoming connections, and <password> with the password required for authentication.</i><br>

## Usage
Once the server is running, users can connect to it using an IRC client and communicate with each other. The following features are supported:<br>

- Authentication with password
- Setting a nickname and username
- Joining a chat channel
- Sending and receiving private messages
- Broadcasting messages to all users in a chat channel

To connect to the server, you can use a client such as IRSSI by running the following command:<br>
`` irssi <server-ip> <server-port> `` <br>
or `` irssi <server-ip> <server-port> <nickname> `` if you want to precise a specific nickname.

Note that IRSSI is the reference client for connecting to the server, as it has been thoroughly tested and is known to work reliably. 
While it is possible to connect with other clients such as netcat, the behavior may be different and could lead to undefined behavior.<br>

``nc <server-ip> <server-port>``

If you choose to use a different client, please be aware that the behavior of the server may not be consistent 
with what you would expect when using IRSSI.

## Code Structure

The code is structured into several files reprouping classes:<br>
- The Server class is the main class of the server program, which manages the server's connection, clients, and channels.<br>
- The Client class represents a client that has connected to the server. It contains information about the client's connection and handles communication with the client.<br>
- The Channel class represents a channel that clients can join to communicate with each other. It contains a list of clients that are currently in the channel and handles sending messages to all clients in the channel.<br>
- The Server class contains methods for managing clients and channels, including adding and removing them, as well as methods for checking the state of the server and debugging.<br>
- The Server class also contains a poll loop that listens for incoming client connections and messages, and handles them accordingly.<br>

## Rules and Constraints

Please note that the following rules and constraints apply:<br>

- The program must not crash or terminate unexpectedly, except in cases of undefined behavior.<br>
- The provided Makefile must compile the code without relinking and should include the rules all, clean, fclean, and re.<br>
- The code must comply with the C++ 98 standard and compile with the -std=c++98 flag.<br>
- External libraries or the Boost library are not allowed.<br>
- Only standard C++ functions are allowed.<br>

## Useful Ressources 

- Official Documentation IRC : <https://www.rfc-editor.org/rfc/rfc1459><br>
- General : <https://oramind.com/tutorial-how-to-make-an-irc-server-connection/> <br><br>
- Socket Programming : <br>
<http://sdz.tdct.org/sdz/les-sockets.html><br>
<https://www.youtube.com/watch?v=bdIiTxtMaKA><br>
<https://www.youtube.com/watch?v=esXw4bdaZkc><br>
<https://www.geeksforgeeks.org/socket-programming-cc/><br><br>
- IRSSI :<br>
<https://irssi.org/download/> <br>
<https://www.linode.com/docs/guides/using-irssi-for-internet-relay-chat/>
