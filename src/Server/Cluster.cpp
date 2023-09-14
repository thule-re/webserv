/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tony <tony@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:03 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 14:18:50 by tony             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server/Cluster.hpp"



Cluster::Cluster() {
	_maxFd = 0;
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
}

Cluster::Cluster(const Cluster &) {

}

Cluster::~Cluster() {

}

Cluster &Cluster::operator=(const Cluster &other) {
	_serverMap = other._serverMap;
	_clientsMap = other._clientsMap;

	_readSet = other._readSet;
	_writeSet = other._writeSet;
	_readSetCopy = other._readSetCopy;
	_writeSetCopy = other._writeSetCopy;

	_maxFd = other._maxFd;

	return *this;
}

void Cluster::initializeServers() {
	Server s1(80, "garbage.html", "www", "www");
	Server s2(85, "upload.html", "www", "www");

	s1.init();
	s2.init();

	_serverMap[s1.getServerSocket()] = s1;
	_serverMap[s2.getServerSocket()] = s2;

	if (s1.getServerSocket() > s2.getServerSocket())
		_maxFd = s1.getServerSocket();
	else
		_maxFd = s2.getServerSocket();

	FD_SET(s1.getServerSocket(), &_readSet);
	FD_SET(s2.getServerSocket(), &_readSet);
}

void Cluster::loop() {
	while (true)
	{
		try {
			selectClientSockets();
			for (int fd = 0; fd <= _maxFd + 1; fd++)
			{
				if (FD_ISSET(fd, &_readSetCopy)) {
					if(_serverMap.count(fd))
						addConnectionToServer(fd);
					else
						readRequestFromClient(fd);
				}
				if (FD_ISSET(fd, &_writeSetCopy))
					sendResponseToServer(fd);
			}
		}
		catch (std::exception &e) {
			handleLoopException(e);
		}
	}
}

void Cluster::selectClientSockets()
{
	struct timeval timeout = {};
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	FD_ZERO(&_readSetCopy);
	FD_ZERO(&_writeSetCopy);
	_readSetCopy = _readSet;
	_writeSetCopy = _writeSet;

	int selectResult = select(_maxFd + 1, &_readSetCopy, &_writeSetCopy, NULL, &timeout);

	if (selectResult == -1) {
		std::cerr << "Error in select" << std::endl;
	}
}

void Cluster::addConnectionToServer(int serverFd) {
	ClientSocket clientSocket = _serverMap[serverFd].addNewConnection();
	int socketFd = clientSocket.getSocketFd();
	FD_SET(socketFd, &_readSet);
	if (socketFd > _maxFd)
		_maxFd = socketFd;
	addClientToMap(clientSocket);
}


void Cluster::addClientToMap(ClientSocket clientSocket) {
	int socketFd = clientSocket.getSocketFd();

	if (_clientsMap.count(socketFd) != 0)
		_clientsMap.erase(socketFd);

	_clientsMap[socketFd] = clientSocket;
	_clientsMap[socketFd].setConnectionTime(std::time(NULL));
}

void Cluster::readRequestFromClient(int socketFd) {
	int clientServer = _clientsMap[socketFd].getServerFd();
	ClientSocket socket = _serverMap[clientServer].setupClient(socketFd);

	socket.readRequest();
	if (socket.isCompleteRequest())
	{
		FD_CLR(socketFd, &_readSet);
		FD_SET(socketFd, &_writeSet);
		Response r = _serverMap[clientServer].process(socket.getSocketFd(), socket);
		socket.setResponse(r);
		addClientToMap(socket);
	}
}

void Cluster::sendResponseToServer(int clientFd) {
	_clientsMap[clientFd].sendResponse();
	closeConnection(_clientsMap[clientFd]);
	_clientsMap.erase(clientFd);
}


void Cluster::handleLoopException(std::exception &exception) {
	std::cerr << exception.what() << std::endl;
}

void Cluster::closeConnection(ClientSocket socket)
{
	int socketFd = socket.getSocketFd();
	if (FD_ISSET(socketFd, &_readSet))
		FD_CLR(socketFd, &_readSet);
	if (FD_ISSET(socketFd, &_writeSet))
		FD_CLR(socketFd, &_writeSet);
	socket.closeSocket();
}

