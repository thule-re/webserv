
#ifndef WEBSERV_CLUSTER_HPP
#define WEBSERV_CLUSTER_HPP

#include <iostream>
#include <map>
#include "Server.hpp"
#include "../Socket/ClientSocket.hpp"
#include "../Parser/Config.hpp"


class Cluster {
	public:
		// constructors
		Cluster();

		Cluster(std::vector<Config> &configs);


	Cluster(const Cluster &);

		// destructor
		~Cluster();

		// operator overload
		Cluster &operator=(const Cluster &other);

		//member functions
		// void initializeServers(); //!alert: function not implemented
		void selectClientSockets();
		void loop();
		void addClientToMap(ClientSocket *clientSocket);


		private:
		std::map<int, Server> _serverMap;
		std::map<int, ClientSocket *> _clientsMap;

		fd_set		_readSet;
		fd_set		_writeSet;
		fd_set		_readSetCopy;
		fd_set		_writeSetCopy;

		int _maxFd;

		void handleLoopException(std::exception &exception);
		void closeConnection(ClientSocket *socket);

	void addConnectionToServer(int);
	void readRequestFromClient(int);
	void sendResponseToClient(int);
	void initializeServer(Config &config);
};


#endif //WEBSERV_CLUSTER_HPP
