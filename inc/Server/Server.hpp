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

	ClientSocket addNewConnection();
	ClientSocket setupClient(int clientSocket);

	void closeConnection(int clientSocket);

	void closeConnection(ClientSocket socket);

	int getServerSocket();

	ClientSocket process(int socketId, ClientSocket socket);

private:
	// member functions
	void selectClientSockets();
	void handleLoopException(std::exception &exception);
	void handleARequestException(ARequest::ARequestException &, Response &);
	void initializeServerSocket();
	void setServerSocketOptions(sockaddr_in *serverAddress);
	void listenOnServerSocket();
	void bindServerSocket(sockaddr_in serverAddress);


	// member variables
	int			_port;
	int			_serverSocket;
	int			_maxFd;

	std::string	_indexPath;
	std::string	_errorPath;
	std::string	_root;

};

#endif
