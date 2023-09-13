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
    _socketFd = other.getSocketFd();
	_indexFile = other._indexFile;
	_rootFolder = other._rootFolder;
	_errorFolder = other._errorFolder;
	_cgiFolder = other._cgiFolder;
	_uploadFolder = other._uploadFolder;
	_serverName = other._serverName;
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

std::string ClientSocket::getIndexFile() const {
	return (_indexFile);
}

std::string ClientSocket::getRootFolder() const {
	return (_rootFolder);
}

std::string ClientSocket::getErrorFolder() const {
	return (_errorFolder);
}

std::string ClientSocket::getCgiFolder() const {
	return (_cgiFolder);
}

std::string ClientSocket::getUploadFolder() const {
	return (_uploadFolder);
}

std::string ClientSocket::getServerName() const {
	return (_serverName);
}

time_t ClientSocket::getConnectionTime() const {
    return _connectionTime;
}

Response ClientSocket::getResponse() const {
	return *_response;
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

void ClientSocket::setIndexFile(const std::string &indexFile) {
	_indexFile = indexFile;
}

void ClientSocket::setIndexFolder(const std::string &indexFolder) {
	_rootFolder = indexFolder;
}

void ClientSocket::setErrorFolder(const std::string &errorFolder) {
	_errorFolder = errorFolder;
}

void ClientSocket::setCgiFolder(const std::string &cgiFolder) {
	_cgiFolder = cgiFolder;
}

void ClientSocket::setUploadFolder(const std::string &uploadFolder) {
	_uploadFolder = uploadFolder;
}

void ClientSocket::setServerName(const std::string &serverName) {
	_serverName = serverName;
}

void ClientSocket::setResponse(const Response& response) {
	_response = new Response(response);
}

void ClientSocket::setConnectionTime(const time_t &connectionTime) {
    _connectionTime = connectionTime;
}

void ClientSocket::setServerFd(int serverFd) {
	_serverFd = serverFd;
}

int ClientSocket::getServerFd()
{
	return _serverFd;
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
