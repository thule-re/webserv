/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tony <tony@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:03 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 14:18:50 by tony             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"

// constructors
Server::Server(): _port(80), _serverSocket() {
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);}

Server::Server(int port, const std::string& error): _port(port), _serverSocket(), _errorPath(error) {
    FD_ZERO(&_readSet);
    FD_ZERO(&_writeSet);
}

Server::Server(int port, const std::string& index, const std::string& error, const std::string& folder): _port(port), _serverSocket(), _indexPath(index), _errorPath(error), _root(folder) {
}

Server::Server(const Config &config):
	_port(std::stoi(config.getMap()["port"])),
	_serverSocket(),
	_indexPath(config.getMap()["indexDirectory"]),
	_errorPath(config.getMap()["errorDirectory"]),
	_root(config.getMap()["root"]) {
}

Server::Server(const Server &other) : _port(), _serverSocket() {
	*this = other;
}

// destructor
Server::~Server() {}

// operator overload
Server &Server::operator=(const Server &other) {
	if (this == &other)
		return (*this);
	_port = other._port;
	_serverSocket = other._serverSocket;
	_readSet = other._readSet;
	_writeSet = other._writeSet;
	_clientsMap = other._clientsMap;
	_locationMap = other._locationMap;

	return (*this);
}

// member functions
void	Server::init() {
	struct sockaddr_in serverAddress = {};

	_locationMap["/"] = Location("/", "www/", "garbage.html", "cgi-bin", "upload", false);
	_locationMap["/test"] = Location("/test", "www", "", "cgi-bin", "upload", true);

	initializeServerSocket();
	setServerSocketOptions(&serverAddress);
	bindServerSocket(serverAddress);
	listenOnServerSocket();

	std::cout << "Server listening on port " << _port << std::endl;
	_maxFd = _serverSocket;
}

void Server::initializeServerSocket() {
	int opt = 1;

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0) {
		std::cerr << "Error creating socket" << std::endl;
		exit(1);
	}
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		std::cerr << "Error setting socket options" << std::endl;
		exit(1);
	}
}

void Server::setServerSocketOptions(sockaddr_in *serverAddress) {
	serverAddress->sin_family = AF_INET;
	serverAddress->sin_addr.s_addr = INADDR_ANY;
	serverAddress->sin_port = htons(_port);
}

void Server::bindServerSocket(sockaddr_in serverAddress) {
	if (bind(_serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Error binding socket" << std::endl;
		exit(1);
	}
}

void Server::listenOnServerSocket() {
	if (listen(_serverSocket, MAX_CLIENT_CONNECTIONS) < 0) {
		std::cerr << "Error listening on socket" << std::endl;
		exit(1);
	}
}

ClientSocket Server::addNewConnection()
{
	int clientSocket = accept(_serverSocket, NULL, NULL);
	if (clientSocket > _maxFd)
		_maxFd = clientSocket;
	if (clientSocket < 0) {
		std::cerr << "Error accepting connection" << std::endl;
	}
	else {
		fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	}

	ClientSocket c(clientSocket);
	c.setServerFd(_serverSocket);

	return c;
}


ClientSocket Server::setupClient(int clientSocket) {
    ClientSocket socket(clientSocket);

    socket.setServerFd(_serverSocket);
    socket.setAllowedHTTPVersion(HTTP_VERSION);
    socket.addToAllowedMethods(METHOD_GET);
    socket.addToAllowedMethods(METHOD_POST);
    socket.addToAllowedMethods(METHOD_DELETE);
    socket.setLocationMap(&_locationMap);

    return socket;
}

Response *Server::process(int socketId, ClientSocket socket)
{
	Response *response;
	ARequest *request;

	try {
		request = ARequest::newRequest(&socket);
		response = request->handle();
	}
	catch (ARequest::ARequestException &e) {
		response = new Response(&socket);
		std::cout << "Handling exception... " << std::endl;
		handleARequestException(e, response);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		response = new Response(&socket);
		response->buildErrorPage(INTERNAL_SERVER_ERROR);
	}
	delete request;
    return response;
}

void Server::closeConnection(int clientSocket)
{
	if (FD_ISSET(clientSocket, &_readSet))
		FD_CLR(clientSocket, &_readSet);
	if (FD_ISSET(clientSocket, &_writeSet))
		FD_CLR(clientSocket, &_writeSet);
	_clientsMap[clientSocket].closeSocket();
	_clientsMap.erase(clientSocket);
}

int Server::getServerSocket() {
	return _serverSocket;
}

void Server::handleARequestException(ARequest::ARequestException &exception, Response *response) {
	std::cerr << exception.message() << std::endl;
	response->buildErrorPage(exception.code());
}
