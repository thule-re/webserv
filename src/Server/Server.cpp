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
Server::Server(): _port(80), _serverSocket(), _indexPath("garbage.html"), _errorPath("error404.html"), _indexFolder("www") {}

Server::Server(int port, const std::string& index, const std::string& error, const std::string& folder): _port(port), _serverSocket(), _indexPath(index), _errorPath(error), _indexFolder(folder) {}

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
	_indexFolder = other._indexFolder;

	return (*this);
}

// member functions
void	Server::init() {
	struct sockaddr_in serverAddr = {};
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
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(_port);
	if (bind(_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		std::cerr << "Error binding socket" << std::endl;
		exit(1);
	}
	if (listen(_serverSocket, 10) < 0) {
		std::cerr << "Error listening on socket" << std::endl;
		exit(1);
	}
	std::cout << "Server listening on port " << _port << std::endl;
}

void	Server::loop() {
	while (1) {
		int clientSocket = accept(_serverSocket, NULL, NULL);
		if (clientSocket < 0) {
			std::cerr << "Error accepting connection" << std::endl;
			exit(1);
		}
		handleRequest(clientSocket);
	}
}

void Server::handleRequest(int clientSocket) {
	ClientSocket client(clientSocket);

	client.setAllowedHTTPVersion(HTTP_VERSION);
	client.addToAllowedMethods(METHOD_GET);
	client.addToAllowedMethods(METHOD_POST);
	client.addToAllowedMethods(METHOD_DELETE);
	client.setIndexFile(_indexPath);
	client.setIndexFolder(_indexFolder);
	client.setErrorFolder(_indexFolder);

	ARequest *request = ARequest::newRequest(client, client.readRequest());
	Response response = request->handle();
	response.send();
	client.closeSocket();
	delete request;
}
