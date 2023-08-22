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
	return (*this);
}

// member functions
void Response::send() {
	std::string response = _header + CRLF + _body;
	::send(_clientSocket.getSocketFd(), response.c_str(), response.length(), 0);
}

void Response::setStatusCode(int statusCode) {
	_statusCode = toString(statusCode);
	_statusMessage = getHTTPErrorMessages(statusCode);
	_header = std::string(HTTP_VERSION) + " " + _statusCode + " " + _statusMessage + CRLF;
}

void Response::setBody(const std::string &body) {
	_body = body;
}

void Response::buildErrorPage(int statusCode) {
	std::string path = _clientSocket.getErrorFolder() + "/error" + toString(statusCode) + ".html";
	setStatusCode(statusCode);
	setContentType("text/html");
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

void Response::setContentType(const std::string &contentType) {
	_contentType = contentType;
	_header += "Content-Type: " + _contentType + CRLF;

}

std::string getHTTPErrorMessages(int statusCode) {
	switch (statusCode) {
		case 200: return MSG_200;
		case 201: return MSG_201;
		case 202: return MSG_202;
		case 204: return MSG_204;
		case 206: return MSG_206;
		case 300: return MSG_300;
		case 301: return MSG_301;
		case 302: return MSG_302;
		case 303: return MSG_303;
		case 304: return MSG_304;
		case 305: return MSG_305;
		case 307: return MSG_307;
		case 400: return MSG_400;
		case 401: return MSG_401;
		case 402: return MSG_402;
		case 403: return MSG_403;
		case 404: return MSG_404;
		case 405: return MSG_405;
		case 406: return MSG_406;
		case 407: return MSG_407;
		case 408: return MSG_408;
		case 409: return MSG_409;
		case 410: return MSG_410;
		case 411: return MSG_411;
		case 412: return MSG_412;
		case 413: return MSG_413;
		case 414: return MSG_414;
		case 415: return MSG_415;
		case 416: return MSG_416;
		case 417: return MSG_417;
		case 500: return MSG_500;
		case 501: return MSG_501;
		case 502: return MSG_502;
		case 503: return MSG_503;
		case 504: return MSG_504;
		case 505: return MSG_505;
		default: return "Unknown HTTP Error";
	}
}

