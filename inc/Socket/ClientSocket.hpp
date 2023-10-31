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
# include "Location/Location.hpp"

# define BUFFER_SIZE 1024

class Response;

class ClientSocket {
public:
	// constructors
	ClientSocket();
	ClientSocket(int socketFd);
	ClientSocket(const ClientSocket &);

	// destructor
	~ClientSocket();

	// operator overload
	ClientSocket &operator=(const ClientSocket &);

	// getter functions
	int getSocketFd() const;
	std::string getAllowedHTTPVersion() const;
	std::string getAllowedMethods() const;
	std::string getRawRequest() const;
	std::string getErrorFolder() const;
	std::string getServerName() const;
	time_t getConnectionTime() const;
	Response *getResponse() const;
	std::map<std::string, Location> *getLocationMap() const;
    int getServerFd();

	// setter functions
	void setRawRequest(const std::string &rawRequest);
	void setAllowedHTTPVersion(const std::string &allowedHTTPVersion);
	void setAllowedMethods(const std::string &allowedMethods);
	void addToAllowedMethods(const std::string &allowedMethods);
	void setErrorFolder(const std::string &errorFolder);
    void setServerName(const std::string &serverName);
	void setResponse(Response* response);
    void setConnectionTime(const time_t &connectionTime);
	void setLocationMap(std::map<std::string, Location> *locationMap);
    void setServerFd(int i);

	// member functions
	void sendResponse();
	ssize_t readRequest();
	void closeSocket() const;
	bool isCompleteRequest() const;



private:
	int								_socketFd;
	int 							_serverFd;

	std::string						_allowedHTTPVersion;
	std::string						_allowedMethods;
	std::string						_rawRequest;
	std::string						_errorFolder;
	std::string						_serverName;

	std::map<std::string, Location>	*_locationMap;
	Response						*_response;

	time_t							_connectionTime;
};

#endif
