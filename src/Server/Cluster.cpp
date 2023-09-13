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
	_serverMap[80] = Server(80, "garbage.html", "www", "www");
//	_serverMap[85] = Server(85, "upload.html", "www", "www");

	_maxFd = 80;
	std::map<int, Server>::iterator it = _serverMap.begin();

	while (it != _serverMap.end()) {
		it->second.init();
		FD_SET(it->first, &_readSet);
		it++;
	}
}

void Cluster::addClientToMap(int clientSocket) {
	ClientSocket newClient(clientSocket);

	if (_clientsMap.count(clientSocket) != 0)
		_clientsMap.erase(clientSocket);

	_clientsMap[clientSocket] = newClient;
	_clientsMap[clientSocket].setConnectionTime(std::time(NULL));
}

void Cluster::loop() {
	while (true)
	{
		try {
			selectClientSockets();
			std::cout << "Max fd: " << _maxFd << std::endl;
			for (int i = 0; i <= _maxFd + 1; i++)
			{
				if (FD_ISSET(i, &_readSetCopy)) {
					if(_serverMap.count(i))
					{
						int clientSocket = _serverMap[i].addNewConnection();
						FD_SET(clientSocket, &_readSet);
						if (clientSocket > _maxFd)
							_maxFd = clientSocket;
						addClientToMap(clientSocket);
					}
					else
					{
						int clientServer = _clientsMap[i].getServerFd();
						_serverMap[clientServer].setupClient(i);
						FD_CLR(i, &_readSet);
						FD_SET(i, &_writeSet);
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

	std::cout << "Select result: " << selectResult << std::endl;
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
