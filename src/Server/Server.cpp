/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:03 by treeps            #+#    #+#             */
/*   Updated: 2023/08/17 11:29:03 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"

// constructors
Server::Server(): _port(80), _serverSocket(), _indexPath("garbage.html"), _errorPath("www"), _root("www") {}

Server::Server(int port, const std::string& index, const std::string& error, const std::string& folder): _port(port), _serverSocket(), _indexPath(index), _errorPath(error), _root(folder) {}

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
	_indexPath = other._indexPath;
	_errorPath = other._errorPath;
	_root = other._root;

	return (*this);
}

// member functions
void	Server::init() {
	struct sockaddr_in serverAddress = {};

	initializeServerSocket();
	setServerSocketOptions(&serverAddress);
	bindServerSocket(serverAddress);
	listenOnServerSocket();
	addServerSocketToPoll();

	std::cout << "Server listening on port " << _port << std::endl;
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
	if (listen(_serverSocket, 10) < 0) {
		std::cerr << "Error listening on socket" << std::endl;
		exit(1);
	}
}

void Server::addServerSocketToPoll()
{
	pollfd serverSocket = {};

	serverSocket.fd = _serverSocket;
	serverSocket.events = POLLIN;
	_clientSockets.push_back(serverSocket);
}

void	Server::loop() {
	while (true) {
		try {
			pollThroughClientSockets();
			addNewConnection();
			handleAnyNewRequests();
		}
		catch (std::exception &e) {
			handleLoopException(e);
		}
	}
}


void Server::pollThroughClientSockets()
{
	int pollResult = poll(_clientSockets.data(), _clientSockets.size(), -1);
	if (pollResult == -1) {
		std::cerr << "Error in poll" << std::endl;
		// break;
		// THROW EXCEPTION instead!
	}
}

void Server::addNewConnection()
{
	pollfd serverSocket = _clientSockets[0];

	if (serverSocket.revents & POLLIN) {
		pollfd clientSocket = {};
		clientSocket.fd = accept(_serverSocket, NULL, NULL);
		if (clientSocket.fd < 0) {
			throw std::runtime_error("Error accepting connection");
		}
		clientSocket.events = POLLIN;
		if (_clientSockets.size() < MAX_CLIENT_CONNECTIONS) {
			_clientSockets.push_back(clientSocket);
		} else {
			close(clientSocket.fd);
			throw std::runtime_error("Connection limit reached. Closing the connection.");
		}
	}
}

void Server::handleAnyNewRequests()
{
	for (size_t i = 1; i < _clientSockets.size(); i++) {
		if (_clientSockets[i].revents & POLLIN && _clientSockets[i].fd != -1)
		{
			handleRequest(_clientSockets[i].fd);
			_clientSockets[i].revents = 0; // Reset events
			removeSocket(i);
		}
	}
}

void Server::removeSocket(size_t i) {
	_clientSockets.erase(_clientSockets.begin() + i); // Remove an element
	_clientSockets.shrink_to_fit();
}

void Server::handleRequest(int clientSocket) {
	ClientSocket client(clientSocket);
	Response response(clientSocket);
	ARequest *request = NULL;

	client.setAllowedHTTPVersion(HTTP_VERSION);
	client.addToAllowedMethods(METHOD_GET);
	client.addToAllowedMethods(METHOD_POST);
	client.addToAllowedMethods(METHOD_DELETE);
	client.setIndexFile(_indexPath);
	client.setIndexFolder(_root);
	client.setErrorFolder(_errorPath);
	client.setUploadFolder("upload");

	client.readRequest();
	try {
		request = ARequest::newRequest(client);
		response = request->handle();
	}
	catch (ARequest::ARequestException &e) {
		handleARequestException(e, response);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		response.buildErrorPage(INTERNAL_SERVER_ERROR);
	}
	delete request;
	response.send();
	client.closeSocket();
}

void Server::handleLoopException(std::exception &exception) {
	std::cerr << exception.what() << std::endl;
}

void Server::handleARequestException(ARequest::ARequestException &exception, Response &response) {
	std::cerr << exception.message() << std::endl;
	response.buildErrorPage(exception.code());
}
