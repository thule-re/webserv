/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tony <tony@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:03 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 14:19:36 by tony             ###   ########.fr       */
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
# include <vector>
# include <fcntl.h>
# include <sys/select.h>
# include <sys/time.h>
# include <map>

# include "Request/ARequest.hpp"
# include "Response/Response.hpp"
# include "Socket/ClientSocket.hpp"

# define MAX_CLIENT_CONNECTIONS 100
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
	void setupClient(int clientSocket);
	void removeSocket(size_t i);
	void addServerSocketToSelect();
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
    int         _maxFd;

	fd_set		_readSet;
    fd_set      _writeSet;
    fd_set      _readSetCopy;
    fd_set      _writeSetCopy;
	std::string	_indexPath;
	std::string	_errorPath;
	std::string	_root;

    std::map<int, ClientSocket> _clientsMap;

    void buildResponse(int clientSocket);
};

#endif
