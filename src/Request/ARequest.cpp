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
#include "Request/RedirectRequest.hpp"

// constructor
ARequest::ARequest() : _location() {}

ARequest::ARequest(ClientSocket *clientSocket): _clientSocket(clientSocket), _rawRequest(clientSocket->getRawRequest()), _location() {
	_header = RequestHeader(_rawRequest.substr(0, _rawRequest.find(CRLF CRLF)));
	std::string host = _header["Host"].substr(0, _header["Host"].find(':'));
	_serverConfig = clientSocket->getServerConfigMap()["default"];
	if (clientSocket->getServerConfigMap().count(host))
		_serverConfig = clientSocket->getServerConfigMap()[host];
	_location = _findLocation(_serverConfig, _header["Path"]);
	if (_header["Path"].find("..") != std::string::npos)
		throw ARequest::ARequestException(FORBIDDEN);
	_expandPath();
	if (_header["Transfer-Encoding"] == "chunked")
		_unchunkBody();
}

ARequest::ARequest(const ARequest &other): _clientSocket(other._clientSocket), _location() {
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
	_location = other._location;
	return (*this);
}

// member functions
ARequest *ARequest::newRequest(ClientSocket *client) {
	std::string request = client->getRawRequest();
	RequestHeader header(request.substr(0, request.find(CRLF CRLF)));
	std::map<std::string, t_serverConfig> serverConfigMap = client->getServerConfigMap();
	t_serverConfig serverConfig = serverConfigMap["default"];
	std::string host = header["Host"].substr(0, header["Host"].find(':'));
	if (serverConfigMap.count(host))
		serverConfig = serverConfigMap[host];
	t_locationConfig locationConfig = _findLocation(serverConfig, header["Path"]);
	if (header["Host"].empty())
		throw ARequest::ARequestException(BAD_REQUEST);
	if (strtol(header["Content-Length"].c_str(), NULL, 10) > g_maxFileSize)
		throw ARequest::ARequestException(REQUEST_ENTITY_TOO_LARGE);
	if (header["Method"].empty() || header["Path"].empty() || header["HTTP-Version"].empty())
		throw ARequest::ARequestException(BAD_REQUEST);
	else if (header["HTTP-Version"] != "HTTP/1.1")
		throw ARequest::ARequestException(HTTP_VERSION_NOT_SUPPORTED);
	else if (locationConfig.allowedMethods.find(header["Method"]) == std::string::npos)
		throw ARequest::ARequestException(METHOD_NOT_ALLOWED);
	else if (!locationConfig.redirect.empty())
		return (new RedirectRequest(client));
	else if (_isCgiPath(serverConfig, header["Path"]))
		return (new CgiRequest(client));
	else if (header["Method"] == "GET")
		return (new GETRequest(client));
	else if (header["Method"] == "POST")
		return (new POSTRequest(client));
	else if (header["Method"] == "DELETE")
		return (new DELETERequest(client));
	else
		throw ARequest::ARequestException(NOT_IMPLEMENTED);
}

// private member functions
bool ARequest::_isCgiPath(const t_serverConfig& serverConfig, const std::string &path) {
	t_locationConfig location = _findLocation(serverConfig, path);

	size_t pos = path.find_last_of('.');
	if (pos == std::string::npos)
		return (false);
	std::string extension = path.substr(pos);
	if (location.cgiExtension.find(extension) != std::string::npos)
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

t_locationConfig ARequest::_findLocation(const t_serverConfig& serverConfig, std::string path) {
	if (path.find('/') == 0)
		path = path.substr(1);
	if (!path.empty())
		path = path.substr(0, path.find('/'));
	path = "/" + path;
	if (serverConfig.locationMap.find(path) != serverConfig.locationMap.end())
		return (serverConfig.locationMap.find(path)->second);
	return (serverConfig.locationMap.find("/")->second);
}

void ARequest::_expandPath() {
	std::string path = _header["Path"];
	if (path.find(_location.path) == 0)
		path.replace(0, _location.path.length(), _location.root + "/");
	if (path[path.length() - 1] == '/' && path.length() > 1)
		path = path.substr(0, path.length() - 1);
	if (_isDirectory(path) && !_location.index.empty())
		path += "/" + _location.index;
	_header["Path"] = path;
}

void ARequest::_unchunkBody() {
	std::string body = _rawRequest.substr(_rawRequest.find(CRLF CRLF) + 4);
	std::string unchunkedBody;
	size_t chunkSize;
	size_t chunkStart;
	while (body.length() > 0) {
		chunkSize = strtol(body.substr(0, body.find(CRLF)).c_str(), NULL, 16);
		if (chunkSize == 0)
			break;
		chunkStart = body.find(CRLF) + 2;
		body = body.substr(chunkStart);
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

int ARequest::ARequestException::code() const {
	return (_code);
}
