/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:03 by treeps            #+#    #+#             */
/*   Updated: 2023/09/28 11:33:10 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listener/Listener.hpp"

// constructors
Listener::Listener() : _port(), _socket(), _maxFd(), _readSet(), _writeSet() {}

Listener::Listener(int port, std::map<std::string, t_serverConfig> &serverConfigMap):
					_port(port),
					_socket(),
					_maxFd(1),
					_readSet(),
					_writeSet(),
					_serverConfigMap(serverConfigMap) {
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
	struct sockaddr_in serverAddress = {};

	initializeSocket();
	setSocketOptions(&serverAddress);
	bindSocket(serverAddress);
	listen();

	std::cout << "Listener listening on port " << _port << std::endl;
	_maxFd = _socket;
}

Listener::Listener(const Listener &other) : _port(), _socket() {
	*this = other;
}

// destructor
Listener::~Listener() {}

// operator overload
Listener &Listener::operator=(const Listener &other) {
	if (this == &other)
		return (*this);
	_port = other._port;
	_socket = other._socket;
	_readSet = other._readSet;
	_writeSet = other._writeSet;
	_maxFd = other._maxFd;
	_serverConfigMap = other._serverConfigMap;

	return (*this);
}

// member functions

void Listener::initializeSocket() {
	int opt = 1;

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0) {
		std::cerr << "Error creating socket" << std::endl;
		exit(1);
	}
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		std::cerr << "Error setting socket options" << std::endl;
		exit(1);
	}
}

void Listener::setSocketOptions(sockaddr_in *serverAddress) const {
	serverAddress->sin_family = AF_INET;
	serverAddress->sin_addr.s_addr = INADDR_ANY;
	serverAddress->sin_port = htons(_port);
}

void Listener::bindSocket(sockaddr_in serverAddress) const {
	if (bind(_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Error binding socket" << std::endl;
		exit(1);
	}
}

void Listener::listen() {
	if (::listen(_socket, MAX_CLIENT_CONNECTIONS) < 0) {
		std::cerr << "Error listening on socket" << std::endl;
		exit(1);
	}
}

ClientSocket *Listener::addNewConnection()
{
	int clientSocket = accept(_socket, NULL, NULL);
	if (clientSocket > _maxFd)
		_maxFd = clientSocket;
	if (clientSocket < 0) {
		std::cerr << "Error accepting connection" << std::endl;
	}
	else {
		fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	}

	ClientSocket *c = new ClientSocket(clientSocket, _serverConfigMap);

	return c;
}

Response *Listener::process(ClientSocket *socket)
{
	Response *response = NULL;
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

int Listener::getSocket() const {
	return _socket;
}

void Listener::handleARequestException(ARequest::ARequestException &exception, Response *response) {
	std::cerr << exception.message() << std::endl;
	response->buildErrorPage(exception.code());
}

