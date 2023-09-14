//
// Created by Anthony Williams on 9/13/23.
//

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

//Cluster &Cluster::operator=(const Cluster &other) {
//	return other;
//}

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

void Cluster::addClientToMap(ClientSocket clientSocket) {
	int socketFd = clientSocket.getSocketFd();

	if (_clientsMap.count(socketFd) != 0)
		_clientsMap.erase(socketFd);

	_clientsMap[socketFd] = clientSocket;
	_clientsMap[socketFd].setConnectionTime(std::time(NULL));
}

void Cluster::loop() {
	while (true)
	{
		try {
			selectClientSockets();
			for (int i = 0; i <= _maxFd + 1; i++)
			{
				if (FD_ISSET(i, &_readSetCopy)) {
					if(_serverMap.count(i))
					{
						ClientSocket clientSocket = _serverMap[i].addNewConnection();
						int socketFd = clientSocket.getSocketFd();
						FD_SET(socketFd, &_readSet);
						if (socketFd > _maxFd)
							_maxFd = socketFd;
						addClientToMap(clientSocket);
					}
					else
					{
						int clientServer = _clientsMap[i].getServerFd();
						ClientSocket socket = _serverMap[clientServer].setupClient(i);
						socket.readRequest();
						if (socket.isCompleteRequest())
						{
							std::cout << "complete request!" << std::endl;
							FD_CLR(i, &_readSet);
							FD_SET(i, &_writeSet);
							_serverMap[clientServer].process(socket.getSocketFd(), socket);
						}
					}
				}
				if (FD_ISSET(i, &_writeSetCopy))
				{
					_clientsMap[i].sendResponse();
					closeConnection(_clientsMap[i]);
					_clientsMap.erase(i);
				}
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
