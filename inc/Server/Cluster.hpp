
#ifndef WEBSERV_CLUSTER_HPP
#define WEBSERV_CLUSTER_HPP

#include <iostream>
#include <map>
#include "Server.hpp"
#include "../Socket/ClientSocket.hpp"


class Cluster {
	public:
		// constructors
		Cluster();

		Cluster(const Cluster &);

		// destructor
		~Cluster();

		// operator overload
		Cluster &operator=(const Cluster &other);

		//member functions
		void initializeServers();

		void selectClientSockets();

		void loop();

		void addClientToMap(ClientSocket clientSocket);


		private:
		std::map<int, Server> _serverMap;
		std::map<int, ClientSocket> _clientsMap;

		fd_set		_readSet;
		fd_set		_writeSet;
		fd_set		_readSetCopy;
		fd_set		_writeSetCopy;

		int _maxFd;

		void handleLoopException(std::exception &exception);

		void closeConnection(ClientSocket socket);

	void addConnectionToServer(int i);

	void readRequestFromClient(int i);

	void sendResponseToServer(int i);
};


#endif //WEBSERV_CLUSTER_HPP
