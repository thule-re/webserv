/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 18:30:56 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 18:30:56 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket/ClientSocket.hpp"
#include "Request/ARequest.hpp"

// constructors
ClientSocket::ClientSocket() : _socketFd(0), _connectionTime(), _response() {}
ClientSocket::ClientSocket(int socketFd, std::map<std::string, t_serverConfig> &serverConfigMap):
							_socketFd(socketFd),
							_connectionTime(time(NULL)),
							_serverConfigMap(serverConfigMap),
							_response(NULL)
							{}
ClientSocket::ClientSocket(const ClientSocket &other) : _socketFd(other._socketFd), _connectionTime(), _response() {
	*this = other;
}

// destructor
ClientSocket::~ClientSocket() {}

// operator overload
ClientSocket &ClientSocket::operator=(const ClientSocket &other) {
	if (this == &other)
		return (*this);
	_socketFd = other._socketFd;
	_connectionTime = other._connectionTime;

	_rawRequest = other._rawRequest;
	_serverConfigMap = other._serverConfigMap;
	_response = other._response;
	return (*this);
}

// getters
int ClientSocket::getSocketFd() const {
	return (_socketFd);
}

std::string ClientSocket::getRawRequest() const {
	return (_rawRequest);
}

time_t ClientSocket::getConnectionTime() const {
    return _connectionTime;
}

Response *ClientSocket::getResponse() const {
	return _response;
}

// setters
void ClientSocket::setRawRequest(const std::string &rawRequest) {
	_rawRequest = rawRequest;
}

void ClientSocket::setResponse(Response* response) {
	_response = response;
}

void ClientSocket::setConnectionTime(const time_t &connectionTime) {
    _connectionTime = connectionTime;
}


// member functions
void ClientSocket::closeSocket() const {
	close(_socketFd);
}

ssize_t ClientSocket::readRequest() {
	char buffer[BUFFER_SIZE];
	ssize_t bytesReceived;

	bytesReceived = recv(_socketFd, buffer, BUFFER_SIZE, 0);
	if (bytesReceived < 0) {
		std::cerr << "Error reading from client socket" << std::endl;
		exit(1);
	}
	_rawRequest += buffer;
	return (bytesReceived);
}

bool ClientSocket::isCompleteRequest() const {
	RequestHeader header(_rawRequest.substr(0, _rawRequest.find(CRLF CRLF)));
	if (header["Method"] == "POST") {
		size_t contentLength = std::strtol(header["Content-Length"].c_str(), NULL, 10);
		if (_rawRequest.substr(_rawRequest.find(CRLF CRLF)).length() < contentLength)
			return (false);
	}
	if (header["Transfer-Encoding"] == "chunked") {
		if (_rawRequest.find("0\r\n\r\n") == std::string::npos)
			return (false);
	}
	return (true);
}

void ClientSocket::sendResponse() {
	_response->send();
	delete _response;
}

std::map<std::string, t_serverConfig> ClientSocket::getServerConfigMap() const{
	return (_serverConfigMap);
}

t_serverConfig ClientSocket::parseServerConfig() {
	RequestHeader header(_rawRequest.substr(0, _rawRequest.find(CRLF CRLF)));
	std::string host = header["Host"];
	if (_serverConfigMap.count(host))
		return (_serverConfigMap[host]);
	return (_serverConfigMap["default"]);
}
