/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                         :+:      :+:    :+:   */
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

class Listener {
public:
	// constructors
	Listener(int port, std::map<std::string, t_serverConfig> &);
	Listener(const Listener &);

	// destructor
	~Listener();

	// operator overload
	Listener &operator=(const Listener &);

	// member functions

	ClientSocket *addNewConnection();

	int getSocket() const;

	Response *process(ClientSocket *socket);

private:
	Listener();
	// member functions
	static void handleARequestException(ARequest::ARequestException &, Response *);
	void initializeSocket();
	void setSocketOptions(sockaddr_in *serverAddress) const;
	void bindSocket(sockaddr_in serverAddress) const;
	void listen();


	// member variables
	int			_port;
	int			_socket;
	int			_maxFd;

	fd_set		_readSet;
	fd_set		_writeSet;

	std::map<std::string, t_serverConfig> _serverConfigMap;
};

#endif
