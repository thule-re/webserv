/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:03 by treeps            #+#    #+#             */
/*   Updated: 2023/08/17 11:29:03 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_SERVER_HPP
# define WEBSERV_SERVER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fstream>

class Server {
public:
	// constructors
	Server();
	Server(int port, std::string index, std::string error, std::string folder);
	Server(const Server &);

	// destructor
	~Server();

	// operator overload
	Server &operator=(const Server &);

	// member functions
	void	init();
	void	loop();

private:
	void	handleRequest(int clientSocket);
	void	handleGETRequest(int clientSocket, std::string request);
	void	handlePOSTRequest(int clientSocket, std::string request);
	void	handleDELETERequest(int clientSocket, std::string request);
	void	handleInvalidRequest(int clientSocket);

	std::string extractPath(std::string request, int start);
	std::string getContentType(std::string path);
	std::string getErrorPage();

	int			_port;
	int			_serverSocket;
	std::string	_indexPath;
	std::string	_errorPath;
	std::string	_indexFolder;
};

#endif
