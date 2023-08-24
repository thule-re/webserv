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
#include "Request/GETRequest.hpp"
#include "Request/POSTRequest.hpp"
#include "Request/DELETERequest.hpp"

// constructor
ARequest::ARequest() : _clientSocket() {}
ARequest::ARequest(const ClientSocket& clientSocket): _clientSocket(clientSocket), _request(clientSocket.getRawRequest()) {}
ARequest::ARequest(const ARequest &other): _clientSocket(other._clientSocket), _request(other._request) {}

// destructor
ARequest::~ARequest() {}

// operator overload
ARequest &ARequest::operator=(const ARequest &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

std::string ARequest::_extractPath(int start) {
	std::string path = _request.substr(start, _request.find(HTTP_VERSION) - (start + 1));
	return path;
}

ARequest *ARequest::newRequest(const ClientSocket &clientSocket) {
	std::string request = clientSocket.getRawRequest();
	std::string version = request.substr(request.find("HTTP/"), request.find("\r\n") - request.find("HTTP/"));
	if (clientSocket.getAllowedHTTPVersion() != version)
		throw ARequest::ARequestException(HTTP_VERSION_NOT_SUPPORTED);
	else if (request.find("GET ") == 0)
		return (new GETRequest(clientSocket));
	else if (request.find("POST ")  == 0)
		return (new POSTRequest(clientSocket));
	else if (request.find("DELETE ") == 0)
		return (new DELETERequest(clientSocket));
	else
		throw ARequest::ARequestException(METHOD_NOT_ALLOWED);
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
