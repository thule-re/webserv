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
	_maxFd = 1;
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);}

Server::Server(int port, const std::string& error): _port(port), _serverSocket(), _errorPath(error) {
	_maxFd = 1;
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
}

Server::Server(const Config &config): _serverSocket() {
	_maxFd = 1;
	_port = atoi(config.getMap()["port"].c_str());
	size_t i = 0;
	while (i < config.getLocations().size()) {
		std::string	key = config.getLocations()[i].getPath();
		_locationMap[key] = config.getLocations()[i];
		i++;
	}
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
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
	_locationMap = other._locationMap;

	return (*this);
}

// member functions
void	Server::init() {
	struct sockaddr_in serverAddress = {};


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

ClientSocket *Server::addNewConnection()
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

	ClientSocket *c = new ClientSocket(clientSocket);
	c->setServerFd(_serverSocket);

	return c;
}


void Server::setupClient(ClientSocket *clientSocket) {
	clientSocket->setServerFd(_serverSocket);

	clientSocket->setAllowedHTTPVersion(HTTP_VERSION);
	clientSocket->addToAllowedMethods(METHOD_GET);
	clientSocket->addToAllowedMethods(METHOD_POST);
	clientSocket->addToAllowedMethods(METHOD_DELETE);
	clientSocket->setLocationMap(&_locationMap);
}

Response *Server::process(ClientSocket *socket)
{
	Response *response;
	ARequest *request = NULL;

	try {
		request = ARequest::newRequest(socket);
		response = request->handle();
	}
	catch (ARequest::ARequestException &e) {
		response = new Response(socket);
		handleARequestException(e, response);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		response = new Response(socket);
		response->buildErrorPage(INTERNAL_SERVER_ERROR);
	}
	delete request;
	return response;
}

int Server::getServerSocket() {
	return _serverSocket;
}

void Server::handleARequestException(ARequest::ARequestException &exception, Response *response) {
	std::cerr << exception.message() << std::endl;
	response->buildErrorPage(exception.code());
}
