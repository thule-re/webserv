/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 18:30:56 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 18:30:56 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_CLIENTSOCKET_HPP
# define WEBSERV_CLIENTSOCKET_HPP

# include <string>
# include <unistd.h>
# include <iostream>
# include <sys/socket.h>
# define BUFFER_SIZE 1024

class ClientSocket {
public:
	// constructors
	ClientSocket();
	ClientSocket(int socketFd);
	ClientSocket(const ClientSocket &);

	// destructor
	~ClientSocket();

	// operator overload
	ClientSocket &operator=(const ClientSocket &);

	// getter functions
	int getSocketFd() const;
	std::string getAllowedHTTPVersion() const;
	std::string getAllowedMethods() const;
	std::string getRawRequest() const;
	std::string getIndexFile() const;
	std::string getIndexFolder() const;
	std::string getErrorFolder() const;
	std::string getCgiFolder() const;
	std::string getUploadFolder() const;
	std::string getServerName() const;

	// setter functions
	void setAllowedHTTPVersion(const std::string &allowedHTTPVersion);
	void setAllowedMethods(const std::string &allowedMethods);
	void addToAllowedMethods(const std::string &allowedMethods);
	void setIndexFile(const std::string &indexFile);
	void setIndexFolder(const std::string &indexFolder);
	void setErrorFolder(const std::string &errorFolder);
	void setCgiFolder(const std::string &cgiFolder);
	void setUploadFolder(const std::string &uploadFolder);
	void setServerName(const std::string &serverName);

	// member functions
	std::string readRequest();
	void closeSocket() const;

private:
	const int	_socketFd;
	std::string	_allowedHTTPVersion;
	std::string	_allowedMethods;
	std::string	_rawRequest;
	std::string	_indexFile;
	std::string	_indexFolder;
	std::string	_errorFolder;
	std::string	_cgiFolder;
	std::string	_uploadFolder;
	std::string	_serverName;
};

#endif
