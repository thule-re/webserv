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
	void	handleRequest(int clientSocket);

	int			_port;
	int			_serverSocket;
	std::string	_indexPath;
	std::string	_errorPath;
	std::string	_indexFolder;
	std::vector<pollfd> _clientSockets;

	void removeSocket(size_t i);

	void setUpServerSocket();

	void pollThroughClientSockets();

	void addNewConnection();

	void handleAnyNewRequests();

	void handleLoopException(std::exception &exception);
};

#endif
