/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 18:30:56 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 18:30:56 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_CLIENTSOCKET_HPP
# define WEBSERV_CLIENTSOCKET_HPP

# include <iostream>
# include <string>
# include <map>
# include <sys/socket.h>
# include <unistd.h>
# include "Parser/Parser.hpp"

# define BUFFER_SIZE 1024

class Response;

class ClientSocket {
public:
	// constructors
	ClientSocket();
	ClientSocket(int socketFd, std::map<std::string, t_serverConfig> &serverConfigMap);
	ClientSocket(const ClientSocket &);

	// destructor
	~ClientSocket();

	// operator overload
	ClientSocket &operator=(const ClientSocket &);

	// getter functions
	int										getSocketFd() const;
	std::string								getRawRequest() const;
	time_t									getConnectionTime() const;
	Response								*getResponse() const;
	std::map<std::string, t_serverConfig>	getServerConfigMap() const;

	// setter functions
	void			setRawRequest(const std::string &rawRequest);
	void			setResponse(Response* response);
	void			setConnectionTime(const time_t &connectionTime);

	// member functions
	void			sendResponse();
	ssize_t			readRequest();
	void			closeSocket() const;
	bool			isCompleteRequest() const;
	t_serverConfig	parseServerConfig();

private:
	int										_socketFd;
	time_t									_connectionTime;

	std::string								_rawRequest;

	std::map<std::string, t_serverConfig>	_serverConfigMap;
	Response								*_response;

};

#endif
