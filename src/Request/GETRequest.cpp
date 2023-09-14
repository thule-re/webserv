/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:11:27 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:11:27 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/GETRequest.hpp"

// constructors
GETRequest::GETRequest() {}
GETRequest::GETRequest(ClientSocket* clientSocket) : ARequest(clientSocket) {}
GETRequest::GETRequest(const GETRequest &other): ARequest(other) {}

// destructor
GETRequest::~GETRequest() {}

// operator overload
GETRequest &GETRequest::operator=(const GETRequest &other) {
	if (this == &other)
		return (*this);
	_header = other._header;
	_clientSocket = other._clientSocket;
	_rawRequest = other._rawRequest;
	return (*this);
}

Response *GETRequest::handle() {
	Response *response = new Response(_clientSocket);
	std::cout << "GETRequest::handle()" << std::endl;

	std::string path = _header["Path"];
	if (_location->getAutoindex() && _isDirectory(path))
		response->setBody(_getDirectoryListing(path));
	else
	{
		std::ifstream file(path.c_str());
		if (!file.is_open()) {
			delete response;
			throw ARequest::ARequestException(NOT_FOUND);
		}
		response->setHeader("Content-Type" ,getContentType(_header["Path"]));
		response->setBody(readFile(file));
	}
	return (response);
}

std::string GETRequest::_getDirectoryListing(const std::string& path) {
	std::string body;
	DIR *dir;
	struct dirent *entry;
	if ((dir = opendir (path.c_str())) != NULL) {
		body += "<html><head><title>Index of " + path + "</title></head><body><h1>Index of " + path + "</h1><hr><pre>";
		entry = readdir (dir);
		while (entry) {
			body += "<a href=\"" + path.substr(_clientSocket.getRootFolder().length()) + "/" + std::string(entry->d_name) + "\">" + entry->d_name + "</a><br>";
			entry = readdir (dir);
		}
		body += "</pre><hr></body></html>";
		closedir (dir);
	} else {
		throw ARequest::ARequestException(NOT_FOUND);
	}
	return (body);
}
