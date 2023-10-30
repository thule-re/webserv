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

# include <cstdio>
# include <cstring>
# include <ctime>
# include <fstream>
# include <iostream>
# include <map>
# include <string>
# include <vector>

# include <arpa/inet.h>
# include <fcntl.h>
# include <netdb.h>
# include <netinet/in.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/types.h>
# include <unistd.h>

# include "Request/ARequest.hpp"
# include "Response/Response.hpp"
# include "Socket/ClientSocket.hpp"
# include "Parser/Config.hpp"

class Server {
public:
	// constructors
	Server();
	Server(int port, const std::string& error);
	Server(const Config &);
	Server(const Server &);

	// destructor
	~Server();

	// operator overload
	Server &operator=(const Server &);

	// member functions
	void	init();

	ClientSocket *addNewConnection();
	void setupClient(ClientSocket *clientSocket);

	int getServerSocket();

	Response *process(ClientSocket *socket);

private:
	// member functions
	void handleARequestException(ARequest::ARequestException &, Response *);
	void initializeServerSocket();
	void setServerSocketOptions(sockaddr_in *serverAddress);
	void listenOnServerSocket();
	void bindServerSocket(sockaddr_in serverAddress);


	// member variables
	int			_port;
	int			_serverSocket;
	int			_maxFd;

	fd_set		_readSet;
	fd_set		_writeSet;

	std::string	_errorPath;

	std::map<std::string, Location>	_locationMap;
};

#endif
