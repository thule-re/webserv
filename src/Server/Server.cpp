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

#include "Server.hpp"

// constructors
Server::Server(): _port(80), _indexPath("garbage.html"), _errorPath("error404.html"), _indexFolder("www") {
	std::cout << "Server default constructor called" << std::endl;
}

Server::Server(int port, std::string index, std::string error, std::string folder): _port(port), _indexPath(index), _errorPath(error), _indexFolder(folder) {
	std::cout << "Server parameter constructor called" << std::endl;
}

Server::Server(const Server &other) {
	std::cout << "Server copy constructor called" << std::endl;
	*this = other;
}

// destructor
Server::~Server() {
	std::cout << "Server destructor called" << std::endl;
}

// operator overload
Server &Server::operator=(const Server &other) {

	std::cout << "Server assignment operator called" << std::endl;
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
	struct sockaddr_in serverAddr;
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
		std::cout << "Connection accepted" << std::endl;
		handleRequest(clientSocket);
	}
}

void Server::handleRequest(int clientSocket) {
	char buffer[1024];
	ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesRead < 0) {
		std::cerr << "Error reading from client socket" << std::endl;
		exit(1);
	}
	std::cout << "Received request:\n" << std::string(buffer, bytesRead) << std::endl;
	std::string stringBuffer = std::string(buffer, bytesRead);
	if (stringBuffer.find("GET ") == 0) {
		handleGETRequest(clientSocket, stringBuffer);
	} else if (stringBuffer.find("POST ") == 0) {
		handlePOSTRequest(clientSocket, stringBuffer);
	} else if (stringBuffer.find("DELETE ") == 0) {
		handleDELETERequest(clientSocket, stringBuffer);
	} else {
		handleInvalidRequest(clientSocket);
	}
	close(clientSocket);
}

void Server::handleGETRequest(int clientSocket, std::string request) {
	std::string path = extractPath(request, 4);
	if (path == "/") {
		path = _indexPath;
	}
	std::string fullPath = _indexFolder + path;
	std::ifstream file(fullPath.c_str());
	std::string response;
	if (file.is_open()) {
		response = "HTTP/1.1 200 OK\r\n"
				   "Content-Type: " + getContentType(path) + "\r\n"
				   "\r\n";
		std::string line;
		while (getline(file, line)) {
			response += line + "\n";
		}
	} else {
		response = "HTTP/1.1 404 Not Found\r\n"
				   "Content-Type: text/html \r\n"
				   "\r\n";
		response += getErrorPage();
	}
	send(clientSocket, response.c_str(), response.size(), 0);
}

void Server::handlePOSTRequest(int clientSocket, std::string request) {
	(void) clientSocket;
	(void) request;
	// TODO: implement
}

void Server::handleDELETERequest(int clientSocket, std::string request) {
	(void) clientSocket;
	(void) request;
	// TODO: implement
}

void Server::handleInvalidRequest(int clientSocket) {
	std::string response = "HTTP/1.1 400 Bad Request\r\n"
						   "Content-Type: text/html \r\n"
						   "\r\n";
	response += getErrorPage();
	send(clientSocket, response.c_str(), response.size(), 0);
}

std::string Server::extractPath(std::string request, int start) {
	std::string path = request.substr(start, request.find("HTTP/1.1") - 5);
	if (path == "/") {
		path += _indexPath;
	}
	return path;
}

std::string Server::getContentType(std::string path) {
	if (path.find(".html") != std::string::npos) {
		return "text/html";
	} else if (path.find(".css") != std::string::npos) {
		return "text/css";
	} else if (path.find(".js") != std::string::npos) {
		return "text/javascript";
	} else if (path.find(".png") != std::string::npos) {
		return "image/png";
	} else if (path.find(".jpg") != std::string::npos) {
		return "image/jpeg";
	} else if (path.find(".gif") != std::string::npos) {
		return "image/gif";
	} else if (path.find(".ico") != std::string::npos) {
		return "image/x-icon";
	} else {
		return "text/plain";
	}
}

std::string Server::getErrorPage() {
	std::ifstream errorFile((_indexFolder + "/" + _errorPath).c_str());
	if (!errorFile.good()) {
		std::cerr << "Error opening error file. Using Default" << std::endl;
		return "<html><body><h1>Default 404 Not Found</h1></body></html>";
	}
	std::string line;
	std::string errorPage;
	while (std::getline(errorFile, line)) {
		errorPage += line;
	}
	return errorPage;
}
