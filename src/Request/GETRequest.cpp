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
GETRequest::GETRequest(const ClientSocket& clientSocket) : ARequest(clientSocket) {
	_method = "GET";
}
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

Response GETRequest::handle() {
	Response response(_clientSocket);
	std::cout << "GETRequest::handle()" << std::endl;
	std::string path = _extractPath(4); // 4 = length of "GET "

	std::ifstream file(path.c_str());
	if (!file.is_open()) {
		throw ARequest::ARequestException(NOT_FOUND);
	}
	response.setStatusCode(OK);
	response.setContentType(getContentType(path));
	response.setBody(readFile(file));
	return (response);
}
