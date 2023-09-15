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
ClientSocket::ClientSocket() : _socketFd(0) {}
ClientSocket::ClientSocket(int socketFd): _socketFd(socketFd) {}
ClientSocket::ClientSocket(const ClientSocket &other) : _socketFd(other._socketFd) {
	*this = other;
}

// destructor
ClientSocket::~ClientSocket() {}

// operator overload
ClientSocket &ClientSocket::operator=(const ClientSocket &other) {
	if (this == &other)
		return (*this);
    _socketFd = other._socketFd;
    _serverFd = other._serverFd;
	_allowedHTTPVersion = other._allowedHTTPVersion;
	_allowedMethods = other._allowedMethods;

	_rawRequest = other._rawRequest;
	_errorFolder = other._errorFolder;
	_serverName = other._serverName;
	_locationMap = other._locationMap;

	_response = other._response;
	_connectionTime = other._connectionTime;

	return (*this);
}

// getters
int ClientSocket::getSocketFd() const {
	return (_socketFd);
}

std::string ClientSocket::getAllowedHTTPVersion() const {
	return (_allowedHTTPVersion);
}

std::string ClientSocket::getAllowedMethods() const {
	return (_allowedMethods);
}

std::string ClientSocket::getRawRequest() const {
	return (_rawRequest);
}

std::string ClientSocket::getErrorFolder() const {
	return (_errorFolder);
}

std::string ClientSocket::getServerName() const {
	return (_serverName);
}

time_t ClientSocket::getConnectionTime() const {
    return _connectionTime;
}

Response *ClientSocket::getResponse() const {
	return _response;
}

std::map<std::string, Location> *ClientSocket::getLocationMap() const {
	return (_locationMap);
}

int ClientSocket::getServerFd()
{
    return _serverFd;
}

// setters
void ClientSocket::setRawRequest(const std::string &rawRequest) {
	_rawRequest = rawRequest;
}

void ClientSocket::setAllowedHTTPVersion(const std::string &allowedHTTPVersion) {
	_allowedHTTPVersion = allowedHTTPVersion;
}

void ClientSocket::setAllowedMethods(const std::string &allowedMethods) {
	_allowedMethods = allowedMethods;
}

void ClientSocket::addToAllowedMethods(const std::string &allowedMethods) {
	_allowedMethods += allowedMethods;
}

void ClientSocket::setErrorFolder(const std::string &errorFolder) {
	_errorFolder = errorFolder;
}

void ClientSocket::setServerName(const std::string &serverName) {
	_serverName = serverName;
}

void ClientSocket::setResponse(Response* response) {
	_response = response;
}

void ClientSocket::setConnectionTime(const time_t &connectionTime) {
    _connectionTime = connectionTime;
}

void ClientSocket::setLocationMap(std::map<std::string, Location> *locationMap) {
	_locationMap = locationMap;
}

void ClientSocket::setServerFd(int serverFd) {
	_serverFd = serverFd;
}


// member functions
void ClientSocket::closeSocket() const {
	close(_socketFd);
}

std::string ClientSocket::readRequest() {
	char buffer[BUFFER_SIZE];

	if (recv(_socketFd, buffer, BUFFER_SIZE, 0) < 0) {
		std::cerr << "Error reading from client socket" << std::endl;
		exit(1);
	}

	_rawRequest += buffer;
	return (buffer);
}

bool ClientSocket::isCompleteRequest() const {
	RequestHeader header(_rawRequest.substr(0, _rawRequest.find(CRLF CRLF)));
	if (header["Method"] == "POST") {
		size_t contentLength = atoi(header["Content-Length"].c_str());
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
