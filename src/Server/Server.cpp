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
	FD_ZERO(&_writeSet);
}

Server::Server(int port, const std::string& error): _port(port), _serverSocket(), _errorPath(error) {
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
	_errorPath = other._errorPath;
	_readSet = other._readSet;
	_writeSet = other._writeSet;
	_clientsMap = other._clientsMap;
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
	FD_SET(_serverSocket, &_readSet);
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

void	Server::loop() {
	while (true) {
		try {
			selectClientSockets();
			for (int i = 0; i <= _maxFd + 1; i++)
			{
				if (FD_ISSET(i, &_readSetCopy)) {
					if (i == _serverSocket)
						addNewConnection();
					else
						setupClient(i);
				}
				if (FD_ISSET(i, &_writeSetCopy))
				{
					_clientsMap[i].sendResponse();
					closeConnection(i);
				}
			}
		}
		catch (std::exception &e) {
			handleLoopException(e);
		}
	}
}


void Server::selectClientSockets()
{
	struct timeval timeout = {};
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	_readSetCopy = _readSet;
	_writeSetCopy = _writeSet;

	int selectResult = select(_maxFd + 1, &_readSetCopy, &_writeSetCopy, NULL, &timeout);

	if (selectResult == -1) {
		std::cerr << "Error in select" << std::endl;
	}
}

void Server::addNewConnection()
{
	int clientSocket = accept(_serverSocket, NULL, NULL);
	if (clientSocket > _maxFd)
		_maxFd = clientSocket;
	if (clientSocket < 0) {
		std::cerr << "Error accepting connection" << std::endl;
	}
	else
	{
		fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		FD_SET(clientSocket, &_readSet);
	}

	ClientSocket newClient(clientSocket);

	if (_clientsMap.count(clientSocket) != 0)
		_clientsMap.erase(clientSocket);

	_clientsMap[clientSocket] = newClient;
	_clientsMap[clientSocket].setConnectionTime(std::time(NULL));
}

void Server::setupClient(int clientSocket) {
	_clientsMap[clientSocket].setAllowedHTTPVersion(HTTP_VERSION);
	_clientsMap[clientSocket].addToAllowedMethods(METHOD_GET);
	_clientsMap[clientSocket].addToAllowedMethods(METHOD_POST);
	_clientsMap[clientSocket].addToAllowedMethods(METHOD_DELETE);
	_clientsMap[clientSocket].setErrorFolder(_errorPath);
	_clientsMap[clientSocket].setLocationMap(&_locationMap);

	_clientsMap[clientSocket].readRequest();
	if (!_clientsMap[clientSocket].isCompleteRequest())
		return;
	else
	{
		process(clientSocket);
		FD_CLR(clientSocket, &_readSet);
		FD_SET(clientSocket, &_writeSet);
	}
}

void Server::process(int clientSocket)
{
	Response *response;
	ARequest *request;

	try {
		request = ARequest::newRequest(&_clientsMap[clientSocket]);
		response = request->handle();
	}
	catch (ARequest::ARequestException &e) {
		response = new Response(&_clientsMap[clientSocket]);
		handleARequestException(e, response);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		response = new Response(&_clientsMap[clientSocket]);
		response->buildErrorPage(INTERNAL_SERVER_ERROR);
	}
	_clientsMap[clientSocket].setResponse(response);
	delete request;
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

void Server::handleLoopException(std::exception &exception) {
	std::cerr << exception.what() << std::endl;
}

void Server::handleARequestException(ARequest::ARequestException &exception, Response *response) {
	std::cerr << exception.message() << std::endl;
	response->buildErrorPage(exception.code());
}
