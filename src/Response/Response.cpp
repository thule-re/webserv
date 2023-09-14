/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:12:06 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:12:06 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response/Response.hpp"

// constructors
Response::Response() {}
Response::Response(const ClientSocket& clientSocket): _clientSocket(clientSocket), _statusCode("-1") {}
Response::Response(const Response &other) {
	*this = other;
}

// destructor
Response::~Response() {}

// operator overload
Response &Response::operator=(const Response &other) {
	if (this == &other)
		return (*this);

	_clientSocket = other._clientSocket;
	_header = other._header;
	_body = other._body;
	_statusCode = other._statusCode;
	_statusMessage = other._statusMessage;
	_contentType = other._contentType;

	return (*this);
}

// member functions
void Response::send() {
	std::string response = _header.exportHeader() + CRLF + _body;
	::send(_clientSocket.getSocketFd(), response.c_str(), response.length(), 0);
}

void Response::setBody(const std::string &body) {
	_body = body;
}

void Response::buildErrorPage(int statusCode) {
	std::string path = _clientSocket.getErrorFolder() + "/error" + toString(statusCode) + ".html";
	_header["HTTP-Status-Code"] = toString(statusCode);
	_header["HTTP-Status-Message"] = getHTTPErrorMessages(statusCode);
	_header["Content-Type"] = "text/html";
	std::ifstream file(path.c_str());
	if (!file.is_open())
		defaultErrorPage(statusCode);
	else
	{
		_body = readFile(file);
		file.close();
	}
}

void Response::defaultErrorPage(int statusCode) {
	_body = DEFAULT_ERROR_PAGE;
	replaceAll(_body, "STATUS_CODE", toString(statusCode));
	replaceAll(_body, "STATUS_MESSAGE", getHTTPErrorMessages(statusCode));
}

std::string Response::getRawResponse() {
	return (_header.exportHeader() + CRLF + _body);
}

void Response::setHeader(const std::string& key, const std::string& value) {
	_header[key] = value;
}

void Response::setHeader(const ResponseHeader &header) {
	_header = header;
}
