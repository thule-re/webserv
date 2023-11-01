
#ifndef WEBSERV_CLUSTER_HPP
# define WEBSERV_CLUSTER_HPP

# include <iostream>
# include <map>
# include "Listener/Listener.hpp"
# include "Socket/ClientSocket.hpp"


class Cluster {
public:
	// constructors
	Cluster();

	Cluster(std::map<int, std::map<std::string, t_serverConfig> >);
	Cluster(const Cluster &);

	// destructor
	~Cluster();

	// operator overload
	Cluster &operator=(const Cluster &other);

	//member functions
	void selectClientSockets();
	void loop();
	void addClientToMap(ClientSocket *clientSocket);


private:

	void handleLoopException(std::exception &exception);
	void closeConnection(ClientSocket *socket);

	void addConnectionToServer(int);
	void readRequestFromClient(int);
	void sendResponseToClient(int);

	std::map<int, Listener *> _listenerMap;
	std::map<int, ClientSocket *> _clientsMap;

	fd_set		_readSet;
	fd_set		_writeSet;
	fd_set		_readSetCopy;
	fd_set		_writeSetCopy;

	int _maxFd;
};


#endif //WEBSERV_CLUSTER_HPP
