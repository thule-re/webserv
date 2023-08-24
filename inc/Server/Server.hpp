/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:03 by treeps            #+#    #+#             */
/*   Updated: 2023/08/17 11:29:03 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_SERVER_HPP
# define WEBSERV_SERVER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fstream>
# include <cstdio>
# include <poll.h>
# include <vector>

# include "Request/ARequest.hpp"
# include "Response/Response.hpp"
# include "Socket/ClientSocket.hpp"

# define MAX_CLIENT_CONNECTIONS 10
# define BUFFER_SIZE 1024

class Server {
public:
	// constructors
	Server();
	Server(int port, const std::string& index, const std::string& error, const std::string& folder);
	Server(const Server &);

	// destructor
	~Server();

	// operator overload
	Server &operator=(const Server &);

	// member functions
	void	init();
	void	loop();

private:
	// member functions
	void handleRequest(int clientSocket);
	void removeSocket(size_t i);
	void addServerSocketToPoll();
	void pollThroughClientSockets();
	void addNewConnection();
	void handleAnyNewRequests();
	void handleLoopException(std::exception &exception);
	void handleARequestException(ARequest::ARequestException &, Response &);
	void initializeServerSocket();
	void setServerSocketOptions(sockaddr_in *serverAddress);
	void listenOnServerSocket();
	void bindServerSocket(sockaddr_in serverAddress);

	// member variables
	int			_port;
	int			_serverSocket;
	std::string	_indexPath;
	std::string	_errorPath;
	std::string	_root;
	std::vector<pollfd> _clientSockets;


};

#endif
