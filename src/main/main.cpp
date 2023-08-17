/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:17 by treeps            #+#    #+#             */
/*   Updated: 2023/08/17 11:29:17 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>

const int PORT = 80;
std::string indexPath = "garbage.html";
std::string errorPath = "error404.html";
std::string indexFolder = "www";

std::string extractPath(std::string request, int start) {
	std::string path = request.substr(start, request.find("HTTP/1.1") - 5);
	if (path == "/") {
		path += indexPath;
	}
	return path;
}

std::string getContentType(std::string path) {
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

std::string getErrorPage() {
	std::ifstream errorFile((indexFolder + "/" + errorPath).c_str());
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

void handleGetRequest(int clientSocket, std::string request) {
	std::string response;
	std::string path = (indexFolder + extractPath(request, 4));
	std::string contentType = getContentType(path);
	std::cout << "Valid request.\nPath: " << path << "\nContent-Type: " << contentType << std::endl << std::endl;
	std::ifstream file(path.c_str());
	std::string line;
	if (file.good()) {
		response = "HTTP/1.1 200 OK\r\nContent-Type: " + contentType + "\r\n\r\n";
		while (std::getline(file, line)) {
			response += line;
		}
	} else {
		std::cerr << "File not found." << std::endl << std::endl;
		response = "HTTP/1.1 404 Not Found\r\n"
				   "Content-Type: text/html \r\n"
				   "\r\n";
		response += getErrorPage();
	}
	send(clientSocket, response.c_str(), response.size(), 0);
	close(clientSocket);
}

int main() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		std::cerr << "Error creating socket." << std::endl;
		return 1;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);

	if (bind(serverSocket, (struct sockaddr *) &serverAddress,
			 sizeof(serverAddress)) == -1) {
		std::cerr << "Error binding to port " << PORT << "." << std::endl;
		return 1;
	}

	if (listen(serverSocket, 10) == -1) {
		std::cerr << "Error listening on socket." << std::endl;
		return 1;
	}

	std::cout << "Server is listening on port " << PORT << "." << std::endl;

	while (true) {
		int clientSocket = accept(serverSocket, NULL, NULL);
		if (clientSocket == -1) {
			std::cerr << "Error accepting client connection." << std::endl;
			continue;
		}

		// Read the request from the client
		char buffer[1024];
		ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead == -1) {
			std::cerr << "Error reading from client socket." << std::endl;
			close(clientSocket);
			continue;
		}

		// Print the received request
		std::cout << "Received Request:\n" << std::string(buffer, bytesRead)
				  << std::endl;
		std::string stringBuffer = std::string(buffer, bytesRead);
		if (stringBuffer.find("GET ") == 0) {
			handleGetRequest(clientSocket, stringBuffer);
		} else {
			std::cerr << "Invalid request." << std::endl;
			std::string response = "HTTP/1.1 400 OK\r\n"
								   "Content-Type: text/html\r\n"
								   "\r\n"
								   "<html><body><h1>400 Bad Request</h1></body></html>";
			send(clientSocket, response.c_str(), response.size(), 0);
			close(clientSocket);
		}
	}

	close(serverSocket);
	return 0;
}
