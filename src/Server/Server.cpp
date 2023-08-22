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
Server::Server(): _port(80), _indexPath("garbage.html"), _errorPath("error404.html"), _indexFolder("www") {
	std::cout << "Server default constructor called" << std::endl;
}

Server::Server(int port, const std::string& index, const std::string& error, const std::string& folder): _port(port), _indexPath(index), _errorPath(error), _indexFolder(folder) {
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
		std::cout << "Connection accepted" << std::endl;
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

//void Server::handleRequest(int clientSocket) {
//	char buffer[BUFFER_SIZE];
//	ssize_t bytesRead;
//	std::string stringBuffer;
//
//	while (true)
//	{
//		bytesRead =  recv(clientSocket, buffer, BUFFER_SIZE, 0);
//		if (bytesRead < 0) {
//			std::cerr << "Error reading from client socket" << std::endl;
//			exit(1);
//		}
//		stringBuffer += std::string(buffer, bytesRead);
//		if (bytesRead < BUFFER_SIZE) {
//			break;
//		}
//	}
//	if (stringBuffer.find("GET ") == 0) {
//		handleGETRequest(clientSocket, stringBuffer);
//	} else if (stringBuffer.find("POST ") == 0) {
//		handlePOSTRequest(clientSocket, stringBuffer);
//	} else if (stringBuffer.find("DELETE ") == 0) {
//		handleDELETERequest(clientSocket, stringBuffer);
//	} else {
//		handleInvalidRequest(clientSocket);
//	}
//	close(clientSocket);
//}

void Server::handleGETRequest(int clientSocket, const std::string& request) {
	std::string path = extractPath(request, 4);
	if (path == "/") {
		path += _indexPath;
	}
	path = _indexFolder + path;
	std::string response;
	std::ifstream file(path.c_str());

	std::cout << "GET request for file: " << path << std::endl;

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

void Server::handlePOSTRequest(int clientSocket, const std::string& request) {
	(void) clientSocket;
	(void) request;
	// TODO: implement
}

void Server::handleDELETERequest(int clientSocket, const std::string& request) {
	std::string path = _indexFolder + extractPath(request, 7);
	std::string response;
	if (path == "/") {
		path += _indexPath;
	}
	path = _indexFolder + path;

	std::cout << "Deleting file: " << path << std::endl;

	if (std::remove(path.c_str()) == 0) {
		response = "HTTP/1.1 200 OK\r\n"
				   "Content-Type: text/html \r\n"
				   "\r\n"
				   "<html><body><h1>File deleted</h1></body></html>";
	} else {
		response = "HTTP/1.1 404 Not Found\r\n"
				   "Content-Type: text/html \r\n"
				   "\r\n";
		response += getErrorPage();
	}
	send(clientSocket, response.c_str(), response.size(), 0);
}

void Server::handleInvalidRequest(int clientSocket) {
	std::string response = "HTTP/1.1 400 Bad ARequest\r\n"
						   "Content-Type: text/html \r\n"
						   "\r\n";
	response += getErrorPage();
	send(clientSocket, response.c_str(), response.size(), 0);
}

std::string Server::extractPath(const std::string& request, int start) {
	std::string path = request.substr(start, request.find("HTTP/1.1") - (start + 1));
	return path;
}

std::string Server::getContentType(const std::string& path) {
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
		return DEFAULT_404;
	}
	std::string line;
	std::string errorPage;
	while (std::getline(errorFile, line)) {
		errorPage += line;
	}
	return errorPage;
}
