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

#include "Server.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int PORT = 80;

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

	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
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

		std::string response = "HTTP/1.1 200 OK\r\n"
							   "Content-Type: text/html\r\n"
							   "\r\n"
							   "<html><body><h1>Hello, Worldasdfsadfas!</h1></body></html>";

		send(clientSocket, response.c_str(), response.size(), 0);
		close(clientSocket);
	}

	close(serverSocket);
	return 0;
}
