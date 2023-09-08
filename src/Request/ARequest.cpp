/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:08:40 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:08:40 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/ARequest.hpp"
#include "Request/CgiRequest.hpp"
#include "Request/DELETERequest.hpp"
#include "Request/GETRequest.hpp"
#include "Request/POSTRequest.hpp"

// constructor
ARequest::ARequest() : _clientSocket() {}

ARequest::ARequest(const ClientSocket& clientSocket): _clientSocket(clientSocket), _rawRequest(clientSocket.getRawRequest()) {
	_header = RequestHeader(_rawRequest.substr(0, _rawRequest.find(CRLF CRLF)));
	if (_header["Path"].find("..") != std::string::npos)
		throw ARequest::ARequestException(FORBIDDEN);
	_expandPath();
	if (_header["Transfer-Encoding"] == "chunked")
		_unchunkBody();
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

// member functions
ARequest *ARequest::newRequest(const ClientSocket &clientSocket) {
	std::string request = clientSocket.getRawRequest();
	RequestHeader header(request.substr(0, request.find(CRLF CRLF)));
	if (header["Method"].empty() || header["Path"].empty() || header["HTTP-Version"].empty())
		throw ARequest::ARequestException(BAD_REQUEST);
	else if (clientSocket.getAllowedHTTPVersion() != header["HTTP-Version"])
		throw ARequest::ARequestException(HTTP_VERSION_NOT_SUPPORTED);
	else if (clientSocket.getAllowedMethods().find(header["Method"]) == std::string::npos)
		throw ARequest::ARequestException(METHOD_NOT_ALLOWED);
	else if (_isCgiPath(clientSocket, header["Path"]))
		return (new CgiRequest(clientSocket));
	else if (header["Method"] == "GET")
		return (new GETRequest(clientSocket));
	else if (header["Method"] == "POST")
		return (new POSTRequest(clientSocket));
	else if (header["Method"] == "DELETE")
		return (new DELETERequest(clientSocket));
	else
		throw ARequest::ARequestException(NOT_IMPLEMENTED);
}

// private member functions
bool ARequest::_isCgiPath(const ClientSocket &clientSocket, const std::string &path) {
	if (path.find("/" + clientSocket.getCgiFolder()) == 0)
		return (true);
	return (false);
}

bool ARequest::_isDirectory(const std::string &path) {
	struct stat s = {};
	if (stat(path.c_str(), &s) == 0)
		if (s.st_mode & S_IFDIR)
			return (true);
	return (false);
}

void ARequest::_expandPath() {
	std::string path = _header["Path"];
	if (path.find(_clientSocket.getRootFolder()) != 0)
		path = _clientSocket.getRootFolder() + path;
	if (path[path.length() - 1] == '/')
		path += _clientSocket.getIndexFile();

	_header["Path"] = path;
}

void ARequest::_unchunkBody() {
	std::string body = _rawRequest.substr(_rawRequest.find(CRLF CRLF) + 4);
	std::string unchunkedBody;
	size_t chunkSize;
	size_t newlinePos;
	while (body.length() > 0) {
		chunkSize = strtol(body.c_str(), NULL, 16);
		if (chunkSize == 0)
			break;
		newlinePos = body.find(CRLF);
		body = body.substr(newlinePos + 2);
		unchunkedBody += body.substr(0, chunkSize);
		body = body.substr(chunkSize + 2);
	}
	_rawRequest = _rawRequest.substr(0, _rawRequest.find(CRLF CRLF) + 4) + unchunkedBody;
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
