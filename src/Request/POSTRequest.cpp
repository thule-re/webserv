/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:11:41 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:11:41 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/POSTRequest.hpp"

// constructors
POSTRequest::POSTRequest() {}
POSTRequest::POSTRequest(const ClientSocket& clientSocket) : ARequest(clientSocket) {}
POSTRequest::POSTRequest(const POSTRequest &other): ARequest(other) {}

// destructor
POSTRequest::~POSTRequest() {}

// operator overload
POSTRequest &POSTRequest::operator=(const POSTRequest &other) {
	if (this == &other)
		return (*this);
	_clientSocket = other._clientSocket;
	_rawRequest = other._rawRequest;
	_header = other._header;
	_boundary = other._boundary;
	_fileData = other._fileData;
	_filename = other._filename;
	return (*this);
}

Response POSTRequest::handle() {
	Response response(_clientSocket);
	std::cout << "POSTRequest::handle()" << std::endl;

	_getBoundary();
	_extractMultipartFormData();
	_getFilename();
	_checkFilename();
	_getFileData();
	_writeDataToOutfile();
	response.setHeader("HTTP-Status-Code", toString(CREATED));
	response.setHeader("HTTP-Status-Message", getHTTPErrorMessages(CREATED));
	return (response);
}

void POSTRequest::_checkFilename()
{
	if (_filename.empty()) {
		throw ARequest::ARequestException(BAD_REQUEST);
	} else if (_filename.find("..") != std::string::npos) {
		throw ARequest::ARequestException(FORBIDDEN);
	}
	_filename = _clientSocket.getRootFolder() + "/" + _clientSocket.getUploadFolder() + "/" + _filename;
}

void POSTRequest::_getBoundary()
{
	_boundary = "--" + _rawRequest.substr(_rawRequest.find("boundary=") + 9);
	size_t newlinePos = _boundary.find('\n');

	_boundary = _boundary.substr(0, newlinePos);
}

void POSTRequest::_extractMultipartFormData()
{
	int clientSocket = _clientSocket.getSocketFd();
	std::string body;
	char buffer[BUFFER_SIZE];
	int bytesRead;

	while (true) {
		bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
		if (bytesRead <= 0) {
			break;
		}
		body.append(buffer, bytesRead);
		size_t pos = body.find(_boundary);
		if (pos != std::string::npos) {
			std::string part = body.substr(0, pos);
			body.erase(0, pos + _boundary.length());
		}
		if ((unsigned long)bytesRead < sizeof(buffer))
			break;
	}
	if (!body.empty())
		_rawRequest = body;
}

void POSTRequest::_getFileData()
{
	size_t contentStart = _rawRequest.find("\r\n\r\n") + 4;
	size_t contentEnd = _rawRequest.find_last_of(_boundary) - _boundary.length() - 4;

	_fileData = _rawRequest.substr(contentStart, contentEnd - contentStart);
}

void POSTRequest::_writeDataToOutfile()
{
	std::ofstream outfile(_filename);

	if (!outfile.is_open()) {
		std::cerr << "Error opening file: " << _filename << std::endl;
		throw ARequest::ARequestException(INTERNAL_SERVER_ERROR);
	}
	outfile << _fileData << std::endl;
	outfile.close();
}

void POSTRequest::_getFilename()
{
	size_t filenamePos;

	filenamePos = _rawRequest.find("filename=\"", 0);
	if (filenamePos != std::string::npos) {
		filenamePos += 10;
		size_t filenameEndPos = _rawRequest.find('\"', filenamePos);
		if (filenameEndPos != std::string::npos) {
			_filename = _rawRequest.substr(filenamePos, filenameEndPos - filenamePos);
		}
	}
	if (_filename.empty())
		throw ARequest::ARequestException(BAD_REQUEST);
}
