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

#include "Server/Server.hpp"

Server::Server() {
	_maxFd = 0;
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
}

Server::Server(std::map<int, std::map<std::string, t_serverConfig> > configMap)
{
	_maxFd = 0;
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);

	for (std::map<int, std::map<std::string, t_serverConfig> >::iterator it = configMap.begin(); it != configMap.end(); it++)
	{
		Listener *listener = new Listener(it->first, it->second);
		int socket = listener->getSocket();

		_listenerMap[socket] = listener;
		if (socket > _maxFd)
			_maxFd = socket;

		FD_SET(socket, &_readSet);
	}
}

Server::Server(const Server &other) : _readSet(), _writeSet(), _readSetCopy(), _writeSetCopy(), _maxFd() {
	*this = other;
}

Server::~Server() {
	for (std::map<int, Listener *>::iterator it = _listenerMap.begin(); it != _listenerMap.end(); it++)
		delete it->second;
}

Server &Server::operator=(const Server &other) {
	_listenerMap = other._listenerMap;
	_clientsMap = other._clientsMap;

	_readSet = other._readSet;
	_writeSet = other._writeSet;
	_readSetCopy = other._readSetCopy;
	_writeSetCopy = other._writeSetCopy;

	_maxFd = other._maxFd;

	return *this;
}

void Server::loop() {
	while (true)
	{
		try {
			selectClientSockets();
			for (int fd = 0; fd <= _maxFd + 1; fd++)
			{
				if (FD_ISSET(fd, &_readSetCopy)) {
					if(_listenerMap[fd] != NULL)
						addConnectionToServer(fd);
					else
						readRequestFromClient(fd);
				}
				if (FD_ISSET(fd, &_writeSetCopy))
					sendResponseToClient(fd);
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
	timeout.tv_sec = g_timeout;
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

void Server::addConnectionToServer(int socket) {
	ClientSocket *clientSocket = _listenerMap[socket]->addNewConnection();
	int clientSocketFD = clientSocket->getSocketFd();
	FD_SET(clientSocketFD, &_readSet);
	if (clientSocketFD > _maxFd)
		_maxFd = clientSocketFD;
	addClientToMap(clientSocket);
}


void Server::addClientToMap(ClientSocket *client) {
	int socketFd = client->getSocketFd();

	if (_clientsMap.count(socketFd) != 0)
		_clientsMap.erase(socketFd);

	_clientsMap[socketFd] = client;
	_clientsMap[socketFd]->setConnectionTime(std::time(NULL));
}

void Server::readRequestFromClient(int clientFd) {
	int listenerFd = _clientsMap[clientFd]->getSocketFd();
	ClientSocket *client = _clientsMap[clientFd];
	_clientsMap[listenerFd]->setConnectionTime(std::time(NULL));

	try {
		client->readRequest();
	} catch (std::exception &e) {
		closeConnection(client);
		delete client;
		_clientsMap.erase(clientFd);
		return;
	}
	if (client->isCompleteRequest())
	{
		FD_CLR(listenerFd, &_readSet);
		FD_SET(listenerFd, &_writeSet);
		Response *r = _listenerMap[listenerFd]->process(client);
		client->setResponse(r);
	}
}

void Server::sendResponseToClient(int clientFd) {
	_clientsMap[clientFd]->sendResponse();
	closeConnection(_clientsMap[clientFd]);
	_clientsMap.erase(clientFd);
}

void Server::handleLoopException(std::exception &exception) {
	std::cerr << exception.what() << std::endl;
}

void Server::closeConnection(ClientSocket *client)
{
	int socketFd = client->getSocketFd();
	if (FD_ISSET(socketFd, &_readSet))
		FD_CLR(socketFd, &_readSet);
	if (FD_ISSET(socketFd, &_writeSet))
		FD_CLR(socketFd, &_writeSet);
	client->closeSocket();
	delete client;
}

