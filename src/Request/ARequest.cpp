/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:08:40 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:08:40 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/ARequest.hpp"
#include "Request/DELETERequest.hpp"
#include "Request/GETRequest.hpp"
#include "Request/POSTRequest.hpp"

// constructor
ARequest::ARequest() : _clientSocket() {}

ARequest::ARequest(const ClientSocket& clientSocket): _clientSocket(clientSocket), _rawRequest(clientSocket.getRawRequest()) {
	_header = RequestHeader(_rawRequest.substr(0, _rawRequest.find("\r\n\r\n")));
	if (_header["Path"].find("..") != std::string::npos)
		throw ARequest::ARequestException(FORBIDDEN);
	else if (_header["Path"] == "/")
		_header["Path"] += _clientSocket.getIndexFile();
	_header["Path"] = _clientSocket.getRootFolder() + _header["Path"];
}

ARequest::ARequest(const ARequest &other) {
	*this = other;
}

// destructor
ARequest::~ARequest() {}

// operator overload
ARequest &ARequest::operator=(const ARequest &other) {
	if (this == &other)
		return (*this);
	_clientSocket = other._clientSocket;
	_rawRequest = other._rawRequest;
	_header = other._header;
	return (*this);
}

ARequest *ARequest::newRequest(const ClientSocket &clientSocket) {
	std::string request = clientSocket.getRawRequest();
	RequestHeader header(request.substr(0, request.find(CRLF CRLF)));
	if (header["Method"].empty() || header["Path"].empty() || header["HTTP-Version"].empty())
		throw ARequest::ARequestException(BAD_REQUEST);
	else if (clientSocket.getAllowedHTTPVersion() != header["HTTP-Version"])
		throw ARequest::ARequestException(HTTP_VERSION_NOT_SUPPORTED);
	else if (clientSocket.getAllowedMethods().find(header["Method"]) == std::string::npos)
		throw ARequest::ARequestException(METHOD_NOT_ALLOWED);
	else if (header["Method"] == "GET")
		return (new GETRequest(clientSocket));
	else if (header["Method"] == "POST")
		return (new POSTRequest(clientSocket));
	else if (header["Method"] == "DELETE")
		return (new DELETERequest(clientSocket));
	else
		throw ARequest::ARequestException(NOT_IMPLEMENTED);
}
// exceptions

ARequest::ARequestException::ARequestException(int code): _code(code) {}

const char *ARequest::ARequestException::what() const throw() {
	return ("ARequestException");
}
std::string ARequest::ARequestException::message() const {
	return (toString(_code) + ": " + getHTTPErrorMessages(_code));
}
int ARequest::ARequestException::code() {
	return (_code);
}
